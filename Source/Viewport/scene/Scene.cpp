#include "Scene.h"

#include "Viewport/Viewport.h"
#include "Viewport/shader/ColorShader.h"
#include "Viewport/shader/GridShader.h"
#include "Viewport/shader/PhongShader.h"

#include "Logger/Logger.h"

using namespace Vp;

Scene::Scene(Viewport* viewport) : m_viewport(viewport)
{
	m_camera = std::make_unique<Camera>();
	m_lighting = std::make_unique<Lighting>();
}

void Scene::draw(int width, int height)
{
	m_camera->size(width, height);
	m_camera->update();

	draw(m_camera->m_view, m_camera->m_projection);
}

void Scene::draw(glm::mat4 view, glm::mat4 projection, const DisplayOptions& displayOptions)
{
	// Setup phong shader, later, shaders are switched for each object
	m_viewport->m_phongShader->use();
	m_viewport->m_phongShader->setUniforms();
	m_lighting->setUniforms(*m_viewport->m_phongShader);

	m_delayedRenderEntities.clear();
	for (auto& entity : m_entities)
	{
		if (!entity->m_visible)
			continue;
		if (!displayOptions.shouldDraw(*entity))
			continue;
		if (!entity->m_opaque)
		{
			m_delayedRenderEntities.push_back(entity.get());
		}
		else
		{
			entity->render(view, projection);
		}
	}

	glm::vec3 cameraPos(glm::inverse(view)[3]);
	// LOG_INFO("Camera pos (V-1): {},{},{}", cameraPos.x, cameraPos.y,
	// cameraPos.z);

	// Sort non-opaque entities by distance from camera
	auto sortByDistanceToCamera = [&](Entity* e1, Entity* e2) -> bool
	{
		// LOG_INFO("Entity pos: {},{},{}", e1->m_modelMatrix[0][3],
		// e1->m_modelMatrix[1][3], e1->m_modelMatrix[2][3]);
		glm::vec3 entity1Pos = glm::vec3(e1->m_modelMatrix[0][3], e1->m_modelMatrix[1][3], e1->m_modelMatrix[2][3]);
		glm::vec3 entity2Pos = glm::vec3(e2->m_modelMatrix[0][3], e2->m_modelMatrix[1][3], e2->m_modelMatrix[2][3]);
		const float e1dist = glm::distance(cameraPos, entity1Pos);
		const float e2dist = glm::distance(cameraPos, entity2Pos);
		return e1dist > e2dist;
	};
	std::sort(m_delayedRenderEntities.begin(), m_delayedRenderEntities.end(), sortByDistanceToCamera);

	// Render the non opaque entities in proper order
	for (auto& entity : m_delayedRenderEntities)
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
	// TODO: Test transparent objects
	CHECK_GL_ERROR();
}

void Scene::update()
{
	for (auto& entity : m_entities)
	{
		entity->update(*this);
	}
}

void Scene::processInput() { m_camera->processInput(); }
