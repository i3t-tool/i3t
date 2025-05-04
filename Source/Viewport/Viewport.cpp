/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Viewport.h"

#include "Core/Resources/ResourceManager.h"
#include "Logger/Logger.h"

#include "Viewport/Shaper.h"
#include "Viewport/camera/AggregateCamera.h"
#include "Viewport/entity/GameObject.h"
#include "Viewport/entity/nodes/SceneCamera.h"
#include "Viewport/entity/nodes/SceneModel.h"
#include "Viewport/scene/SceneRenderTarget.h"
#include "Viewport/scene/scenes/MainScene.h"
#include "Viewport/shader/Shaders.h"

using namespace Vp;

Viewport::Viewport()
{
	// Empty
}

Viewport::~Viewport() = default;

void Viewport::init()
{
	// Load shaders
	Shaders::instance().load();

	// Load shapes
	Shaper::initDefaultShapes();

	// Preload some useful models
	RMI.mesh("Data/Models/camera.gltf");

	// Create manipulators
	m_manipulators = std::make_shared<Manipulators>(this);

	// Setup scenes
	m_mainScene = std::make_shared<MainScene>(this);
	m_previewScene = std::make_shared<PreviewScene>(this);

	m_mainScene->init();
	m_previewScene->init();

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Viewport::drawViewport(Ptr<SceneRenderTarget>& renderTarget, int width, int height, const glm::mat4& model,
                            const RenderOptions& renderOptions, const DisplayOptions& displayOptions)
{
	prepareRenderTarget(renderTarget, renderOptions);
	SceneRenderTarget& rt = *renderTarget;
	m_mainScene->draw(width, height, model, *renderTarget, displayOptions);
}

void Viewport::drawViewport(Ptr<SceneRenderTarget>& renderTarget, int width, int height,
                            const std::shared_ptr<AbstractCamera>& camera, const glm::mat4& model,
                            const RenderOptions& renderOptions, const DisplayOptions& displayOptions)
{
	prepareRenderTarget(renderTarget, renderOptions);
	m_mainScene->draw(width, height, camera, model, *renderTarget, displayOptions);
}

void Viewport::drawScreen(Ptr<SceneRenderTarget>& renderTarget, int width, int height, const glm::mat4& model,
                          const glm::mat4& view, const glm::mat4& projection, const RenderOptions& renderOptions,
                          const DisplayOptions& displayOptions)
{
	prepareRenderTarget(renderTarget, renderOptions);
	m_mainScene->draw(width, height, model, view, projection, *renderTarget, displayOptions);
}

void Viewport::drawPreview(Ptr<SceneRenderTarget>& renderTarget, int width, int height, WPtr<GameObject> gameObject,
                           const RenderOptions& renderOptions)
{
	if (!renderTarget)
	{
		renderTarget = m_previewScene->createRenderTarget(renderOptions);
	}
	else
	{
		renderTarget->setRenderOptions(renderOptions);
	}
	if (auto gameObjectPtr = gameObject.lock())
	{
		// Center camera on game object
		m_previewScene->m_orbitCamera->setFov(m_settings.global().preview_fov);
		m_previewScene->m_orbitCamera->centerOnBox(gameObjectPtr->m_mesh->m_boundingBoxMin,
		                                           gameObjectPtr->m_mesh->m_boundingBoxMax, false);
		float radius = m_previewScene->m_orbitCamera->getRadius();
		float enlargedRadius = radius * m_settings.global().preview_radiusFactor;
		m_previewScene->m_orbitCamera->setRadius(enlargedRadius);

		// Adjust zFar/zNear
		m_previewScene->m_orbitCamera->setZFar(enlargedRadius * 2);
		m_previewScene->m_orbitCamera->setZNear((enlargedRadius - radius) * 0.1f);

		// Make object visible
		bool visibleTemp = gameObjectPtr->m_visible;
		gameObjectPtr->m_visible = true;

		DisplayOptions displayOptions;
		displayOptions.showFrustum = false;
		displayOptions.showCamera = false;
		displayOptions.showAxes = false;

		// Temporarily add the game object to the preview scene with a different
		// model matrix add/removeEntity methods are deliberately not used here
		glm::mat4 temp = gameObjectPtr->m_modelMatrix;
		gameObjectPtr->m_modelMatrix = glm::mat4(1.0f);
		m_previewScene->m_entities.push_back(gameObjectPtr);
		m_previewScene->draw(width, height, glm::mat4(1.f), *renderTarget, displayOptions);
		m_previewScene->m_entities.pop_back();
		gameObjectPtr->m_modelMatrix = temp;

		// Restore object visibility
		gameObjectPtr->m_visible = visibleTemp;
	}
	else
	{
		LOG_ERROR("Viewport: drawPreview(): Passed game object is NULL!");
	}
}

void Viewport::update(double dt)
{
	m_mainScene->update(dt);

	m_previewScene->m_orbitCamera->setRotationX(m_previewScene->m_orbitCamera->getRotationX() +
	                                            m_settings.global().preview_rotateSpeed * dt);
}

void Viewport::processInput(double dt, glm::vec2 mousePos, glm::ivec2 windowSize)
{
	m_mainScene->processInput(dt, mousePos, windowSize);
}

void Viewport::processSelection(Ptr<SceneRenderTarget> renderTarget, glm::vec2 mousePos, glm::ivec2 windowSize)
{
	m_mainScene->processSelection(*renderTarget, mousePos, windowSize);
}

WPtr<SceneModel> Viewport::createModel(Core::ID guiNodeId)
{
	Core::Mesh* mesh = RM::instance().mesh("Cube", "Data/Models/CubeFixed.gltf");
	auto sceneModel = std::make_shared<SceneModel>("Cube", SHADERS.getShaderPtr<PhongShader>());
	// Ensure that "default_cube" is also an alias, backwards compatibility
	RM::instance().mesh("default_cube", "Data/Models/CubeFixed.gltf");
	sceneModel->m_guiNodeId = guiNodeId;
	m_mainScene->addEntity(sceneModel);
	return sceneModel;
}

WPtr<SceneCamera> Viewport::createCamera(Core::ID guiNodeId)
{
	// TODO: (DR) It'd be nice to use a camera model with a lens assembly looking similar to a frustum
	//  Key word being an old projector or some old grain film movie cameras.
	Core::Mesh* mesh = RM::instance().mesh("Data/Models/camera.gltf");
	auto sceneCamera = std::make_shared<SceneCamera>(mesh, SHADERS.getShaderPtr<PhongShader>());
	sceneCamera->m_guiNodeId = guiNodeId;
	m_mainScene->addEntity(sceneCamera);
	return sceneCamera;
}

ViewportSettings& Viewport::getSettings()
{
	return m_settings;
}

Manipulators& Viewport::getManipulators()
{
	return *m_manipulators;
}
void Viewport::prepareRenderTarget(std::shared_ptr<SceneRenderTarget>& renderTarget, const RenderOptions& renderOptions)
{
	if (!renderTarget)
	{
		renderTarget = m_mainScene->createRenderTarget(renderOptions);
	}
	else
	{
		renderTarget->setRenderOptions(renderOptions);
	}
}
