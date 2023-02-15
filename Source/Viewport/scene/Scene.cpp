#include "Scene.h"

#include "Core/Resources/ResourceManager.h"
#include "Logger/Logger.h"

#include "Viewport/Shaper.h"
#include "Viewport/Viewport.h"
#include "Viewport/shader/ColorShader.h"
#include "Viewport/shader/GridShader.h"
#include "Viewport/shader/PhongShader.h"
#include "Viewport/shader/WBOITCompositeShader.h"

#include "SceneRenderTarget.h"

using namespace Vp;

Scene::Scene(Viewport* viewport) : m_viewport(viewport)
{
	m_camera = std::make_unique<Camera>();
	m_lighting = std::make_unique<Lighting>();
}

void Scene::draw(int width, int height, SceneRenderTarget& renderTarget, const DisplayOptions& displayOptions)
{
	m_camera->size(width, height);
	m_camera->update();

	return draw(width, height, m_camera->m_view, m_camera->m_projection, renderTarget, displayOptions);
}

// TODO: (DR) Find a way to set glClear color and alpha initial value (should be 0 for transparent buffers and
//  1 for others, although maybe it doesnt matter what the alpha is for opaque buffers as they are, well, opaque

void Scene::draw(int width, int height, glm::mat4 view, glm::mat4 projection, SceneRenderTarget& renderTarget,
                 const DisplayOptions& displayOptions)
{
	// Retrieve framebuffers from render target and try to update them with render settings
	bool wboit = renderTarget.getRenderOptions().wboit;
	bool alpha = renderTarget.getRenderOptions().framebufferAlpha;
	glm::vec3 clearColor = renderTarget.getRenderOptions().clearColor;
	bool multisample = renderTarget.getRenderOptions().multisample;
	unsigned int samples = renderTarget.getRenderOptions().samples;

	Ptr<Framebuffer> opaqueFBO = renderTarget.getFramebuffer(0).lock();
	Ptr<Framebuffer> transparentFBO = renderTarget.getFramebuffer(1).lock();

	opaqueFBO->setMultisampled(multisample, samples);
	transparentFBO->setMultisampled(multisample, samples);

	if (wboit)
	{
		// Weighted Blended Order Independent Transparency render (OpenGL 3 compatible)

		// ### 1. Draw opaque objects into a regular texture
		opaqueFBO->start(width, height);
		{
			// Using regular depth test and no blending
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			glClearColor(clearColor.r, clearColor.g, clearColor.b, alpha ? 0.0f : 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// Setup phong shader, later, shaders are switched for each object
			m_viewport->m_shaders->m_phongShader->use();
			m_lighting->setUniforms(*m_viewport->m_shaders->m_phongShader);

			m_unorderedTransparentEntities.clear();
			for (auto& entity : m_entities)
			{
				entity->m_wboit = false; // Not using wboit for opaque pass
				if (!entity->m_visible)
					continue;
				if (!displayOptions.shouldDraw(*entity))
					continue;
				if (!entity->m_opaque)
				{
					m_unorderedTransparentEntities.push_back(entity.get());
				}
				else
				{
					entity->render(view, projection);
				}
				entity->m_wboit = true; // Next pass should be using wboit
			}
		}
		opaqueFBO->end(false);

		// ### 2. Draw transparent objects into accumulation and revealage buffers

		// Transparent fbo might not be initialized or resized yet, so ensure proper state here before depth copy
		transparentFBO->update(width, height);

		// Copy depth buffer from opaque pass to transparent pass
		glBindFramebuffer(GL_READ_FRAMEBUFFER, opaqueFBO->getId());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, transparentFBO->getId());
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		transparentFBO->start(width, height);
		{
			glClear(GL_COLOR_BUFFER_BIT); // Only clear color as depth is copied from opaque framebuffer

			// Using depth test but not doing depth writes, special blending enabled
			glDepthMask(GL_FALSE);
			glEnable(GL_BLEND);
			glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);

			// Clear accumulation and revealage buffers to their respective initial values
			// Note that the buffers are actually mixed
			// Accumulation buffer holds RGB accumulation in its RGB components and revealage factor in its alpha.
			// Revealage buffer holds accumulation alpha in its single channel.
			glClearBufferfv(GL_COLOR, 1, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			glClearBufferfv(GL_COLOR, 2, glm::value_ptr(glm::vec4(0.0f)));

			// Render transparent objects with with their WBOIT flag enabled (enabled after opaque pass)
			for (auto& entity : m_unorderedTransparentEntities)
			{
				entity->render(view, projection);
			}
		}
		transparentFBO->end(false);
		if (transparentFBO->isMultisampled())
		{
			transparentFBO->multisampleResolveColor(1); // Resolve accumulation buffer
			transparentFBO->multisampleResolveColor(2); // Resolve revealage buffer
		}

		// ### 3. Composite transparent pass onto opaque pass
		opaqueFBO->start(width, height);
		{

			// Depth test disabled, regular blending enabled
			glDepthFunc(GL_ALWAYS);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			auto wboitCompositeShader = m_viewport->m_shaders->m_wboitCompositeShader;
			wboitCompositeShader->use();
			wboitCompositeShader->accumulationTextureID = transparentFBO->getColorTexture(1);
			wboitCompositeShader->revealageTextureID = transparentFBO->getColorTexture(2);
			wboitCompositeShader->setUniforms();

			// Render transparent object overlay using a screen quad
			RMI.meshByAlias(Shaper::screenQuad)->render();
		}
		opaqueFBO->end();

		// Return final composite image
		renderTarget.setOutputFramebuffer(WPtr<Framebuffer>(opaqueFBO));
		return;
	}
	else
	{
		// Standard ordered transparency render

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		opaqueFBO->start(width, height);
		{
			glClearColor(clearColor.r, clearColor.g, clearColor.b, alpha ? 0.0f : 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// Setup phong shader, later, shaders are switched for each object
			m_viewport->m_shaders->m_phongShader->use();
			m_lighting->setUniforms(*m_viewport->m_shaders->m_phongShader);

			m_unorderedTransparentEntities.clear();
			m_explicitTransparencyOrderEntitiesFirst.clear();
			m_explicitTransparencyOrderEntitiesLast.clear();
			for (auto& entity : m_entities)
			{
				entity->m_wboit = false; // Not using wboit
				if (!entity->m_visible)
					continue;
				if (!displayOptions.shouldDraw(*entity))
					continue;

				// Render opaque entities
				if (entity->m_opaque)
				{
					entity->render(view, projection);
					continue;
				}
				// Store transparent entities for sorting
				if (entity->m_explicitTransparencyOrder != 0)
				{
					// Entities with manually set transparency order
					if (entity->m_explicitTransparencyOrder < 10000)
					{
						// Rendered BEFORE unordered ones
						m_explicitTransparencyOrderEntitiesFirst.push_back(entity.get());
					}
					else
					{
						// Rendered AFTER unordered ones
						m_explicitTransparencyOrderEntitiesLast.push_back(entity.get());
					}
				}
				else
				{
					// Unordered entities
					m_unorderedTransparentEntities.push_back(entity.get());
				}
			}

			// Sort transparent entities
			// Entities are grouped into 3 lists and drawn in this order
			// 1. Explicitly ordered entities (with manually set order) with order < 10000 (sorted in ascending order)
			// 2. Unordered entities (with order = 0) (Sorted by distance to camera)
			// 3. Explicitly ordered entities with order >= 10000 (sorted in ascending order)

			sortExplicitlyOrderedTransparentEntities(m_explicitTransparencyOrderEntitiesFirst);
			sortUnorderedTransparentEntities(view, m_unorderedTransparentEntities);
			sortExplicitlyOrderedTransparentEntities(m_explicitTransparencyOrderEntitiesLast);

			// Don't write depth, we want to see transparent entities through each other even when its wrong
			glDepthMask(GL_FALSE);

			renderSortedTransparentEntities(view, projection, m_explicitTransparencyOrderEntitiesFirst);
			renderSortedTransparentEntities(view, projection, m_unorderedTransparentEntities);
			renderSortedTransparentEntities(view, projection, m_explicitTransparencyOrderEntitiesLast);
		}
		opaqueFBO->end();

		// Return framebuffer
		renderTarget.setOutputFramebuffer(WPtr<Framebuffer>(opaqueFBO));
		return;
	}
}

Ptr<SceneRenderTarget> Scene::createRenderTarget(const RenderOptions& options)
{
	Ptr<SceneRenderTarget> renderTarget = std::make_shared<SceneRenderTarget>();
	renderTarget->setRenderOptions(options);

	Ptr<Framebuffer> opaqueFramebuffer = std::shared_ptr<Framebuffer>(
	    Framebuffer::createDefault(options.multisample, options.samples, options.framebufferAlpha));
	renderTarget->addFramebuffer(opaqueFramebuffer);

	Ptr<Framebuffer> transparentFramebuffer = std::shared_ptr<Framebuffer>(
	    Framebuffer::createDefault(options.multisample, options.samples, options.framebufferAlpha));
	transparentFramebuffer->addColorAttachment(ColorAttachment(GL_RGBA16F, GL_RGBA, 100, 100, GL_HALF_FLOAT));
	transparentFramebuffer->addColorAttachment(ColorAttachment(GL_R16F, GL_RED, 100, 100, GL_HALF_FLOAT));
	renderTarget->addFramebuffer(transparentFramebuffer);

	return renderTarget;
}

void Scene::sortUnorderedTransparentEntities(glm::mat4 view, std::vector<Entity*>& entities)
{
	// Sort by distance to camera
	glm::vec3 cameraPos(glm::inverse(view)[3]);

	auto sortByDistanceToCamera = [&](Entity* e1, Entity* e2) -> bool
	{
		glm::vec3 entity1Pos = glm::vec3(e1->m_modelMatrix[3][0], e1->m_modelMatrix[3][1], e1->m_modelMatrix[3][2]);
		glm::vec3 entity2Pos = glm::vec3(e2->m_modelMatrix[3][0], e2->m_modelMatrix[3][1], e2->m_modelMatrix[3][2]);
		const float e1dist = glm::distance(cameraPos, entity1Pos);
		const float e2dist = glm::distance(cameraPos, entity2Pos);
		return e1dist > e2dist;
	};
	std::sort(m_unorderedTransparentEntities.begin(), m_unorderedTransparentEntities.end(), sortByDistanceToCamera);
}
void Scene::sortExplicitlyOrderedTransparentEntities(std::vector<Entity*>& entities)
{
	auto sortByExplicitTransparencyOrder = [&](Entity* e1, Entity* e2) -> bool
	{ return e1->m_explicitTransparencyOrder > e2->m_explicitTransparencyOrder; };

	std::sort(m_explicitTransparencyOrderEntitiesFirst.begin(), m_explicitTransparencyOrderEntitiesFirst.end(),
	          sortByExplicitTransparencyOrder);
}

void Scene::renderSortedTransparentEntities(glm::mat4 view, glm::mat4 projection,
                                            const std::vector<Entity*>& entities) const
{
	for (const auto& entity : entities)
	{
		if (entity->m_backFaceCull)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		entity->render(view, projection);
		if (entity->m_backFaceCull)
		{
			glDisable(GL_CULL_FACE);
		}
	}
}

void Scene::update()
{
	for (auto& entity : m_entities)
	{
		entity->update(*this);
	}
}

void Scene::processInput(glm::vec2 mousePos, glm::ivec2 windowSize) {
	m_camera->processInput(mousePos, windowSize);
}