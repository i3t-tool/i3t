#include "Viewport.h"

#include "Core/Resources/ResourceManager.h"
#include "Logger/Logger.h"
#include "Utils/Color.h"

#include "Viewport/Shaper.h"
#include "Viewport/camera/AggregateCamera.h"
#include "Viewport/camera/OrbitCamera.h"
#include "Viewport/entity/GameObject.h"
#include "Viewport/entity/nodes/SceneCamera.h"
#include "Viewport/entity/nodes/SceneModel.h"
#include "Viewport/scene/SceneRenderTarget.h"
#include "Viewport/shader/ColorShader.h"
#include "Viewport/shader/FrustumShader.h"
#include "Viewport/shader/GridShader.h"
#include "Viewport/shader/PhongShader.h"
#include "Viewport/shader/WBOITCompositeShader.h"

using namespace Vp;

Viewport::Viewport()
{
	// Empty
}

Viewport::~Viewport() = default;

void Viewport::onInit()
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

void Viewport::drawViewport(Ptr<SceneRenderTarget>& renderTarget, int width, int height,
                            const RenderOptions& renderOptions, const DisplayOptions& displayOptions)
{
	if (!renderTarget)
	{
		renderTarget = m_mainScene->createRenderTarget(renderOptions);
	}
	else
	{
		renderTarget->setRenderOptions(renderOptions);
	}
	m_mainScene->draw(width, height, *renderTarget, displayOptions);
}

void Viewport::drawScreen(Ptr<SceneRenderTarget>& renderTarget, int width, int height, glm::mat4 view,
                          glm::mat4 projection, const RenderOptions& renderOptions,
                          const DisplayOptions& displayOptions)
{
	if (!renderTarget)
	{
		renderTarget = m_mainScene->createRenderTarget(renderOptions);
	}
	else
	{
		renderTarget->setRenderOptions(renderOptions);
	}
	m_mainScene->draw(width, height, view, projection, *renderTarget, displayOptions);
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
		m_previewScene->draw(width, height, *renderTarget, displayOptions);
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

void Viewport::onUpdate(double dt)
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
	Core::Mesh* mesh = RM::instance().mesh("default_cube", "Data/Models/CubeFixed.gltf");
	auto sceneModel = std::make_shared<SceneModel>("default_cube", Shaders::instance().m_phongShader.get());
	sceneModel->m_guiNodeId = guiNodeId;
	m_mainScene->addEntity(sceneModel);
	return sceneModel;
}

WPtr<SceneCamera> Viewport::createCamera(Core::ID guiNodeId)
{
	// TODO: (DR) It'd be nice to use a camera model with a lens assembly looking similar to a frustum
	//  Key word being an old projector or some old grain film movie cameras.
	Core::Mesh* mesh = RM::instance().mesh("Data/Models/camera.gltf");
	auto sceneCamera = std::make_shared<SceneCamera>(mesh, Shaders::instance().m_phongShader.get());
	sceneCamera->m_guiNodeId = guiNodeId;
	m_mainScene->addEntity(sceneCamera);
	return sceneCamera;
}

// TODO: (DR) Should probably be something like viewport->getMainScene()->getCamera() since its not clear which camera
//   we're actually getting here (without reading the doc)
WPtr<AggregateCamera> Viewport::getMainViewportCamera()
{
	std::shared_ptr<AggregateCamera> camera = std::dynamic_pointer_cast<AggregateCamera>(m_mainScene->m_camera);
	return camera;
}

ViewportSettings& Viewport::getSettings()
{
	return m_settings;
}

Manipulators& Viewport::getManipulators()
{
	return *m_manipulators;
}

/////////////////////////////////////////
// State save/load
/////////////////////////////////////////

Memento Viewport::saveScene(State::Scene* scene)
{
	// TODO: (DR) Other things aside from Viewport that could use the saveGlobal/Scene method:
	// - Default models are a global setting and are loaded using the outdated "Config" class
	// - Themes could be rewritten to use this global mechanism rather than the "ThemeLoader" class

	// Update current settings data with current values
	m_mainScene->saveSettings(m_settings, true, false);

	rapidjson::Document doc;
	doc.SetObject();

	// Serialize scene settings
	rapidjson::Document viewportDoc(&doc.GetAllocator());
	auto result = JSON::serializeToDocument(m_settings.scene(), viewportDoc);
	if (!result)
	{
		return emptyMemento();
	}
	doc.AddMember("viewport", viewportDoc, doc.GetAllocator());

	return doc;
}

void Viewport::loadScene(const Memento& memento, State::Scene* scene)
{
	if (!memento.HasMember("viewport"))
	{
		LOG_ERROR("Cannot load scene viewport data! No 'viewport' entry found!");
		return;
	}
	JSON::deserializeDocument(memento["viewport"], m_settings.scene());

	// Update scene values
	m_mainScene->loadSettings(m_settings, true, false);
}

void Viewport::clearScene()
{
	// Clear manipulators
	m_manipulators->clearManipulators();

	// Clear the scene specific settings
	m_settings.scene() = ViewportSceneSettings();

	// Update scene values
	m_mainScene->loadSettings(m_settings, true, false);
}

Memento Viewport::saveGlobal()
{
	rapidjson::Document doc;
	doc.SetObject();

	// Update current settings data with current values
	m_mainScene->saveSettings(m_settings, false, true);

	// Serialize global settings
	rapidjson::Document viewportDoc(&doc.GetAllocator());
	auto result = JSON::serializeToDocument(m_settings.global(), viewportDoc);
	if (!result)
	{
		return emptyMemento();
	}
	doc.AddMember("viewport", viewportDoc, doc.GetAllocator());

	return doc;
}

void Viewport::loadGlobal(const Memento& memento)
{
	if (!memento.HasMember("viewport"))
	{
		LOG_ERROR("Cannot load global viewport data! No 'viewport' entry found!");
		return;
	}
	JSON::deserializeDocument(memento["viewport"], m_settings.global());

	// Update scene values
	m_mainScene->loadSettings(m_settings, false, true);
}

void Viewport::clearGlobal()
{
	// Clear the global settings
	m_settings.global() = ViewportGlobalSettings();

	// Update scene values
	m_mainScene->loadSettings(m_settings, false, true);
};
