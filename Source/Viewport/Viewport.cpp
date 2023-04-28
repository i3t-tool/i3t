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

void Viewport::init(ViewportSettings settings)
{
	m_settings = settings;

	// Load shaders
	Shaders::instance().load();

	// Load shapes
	Shaper::initDefaultShapes();

	// Preload some useful models
	RMI.mesh("Data/Models/super8.gltf");

	// Create manipulators
	m_manipulators = std::make_shared<Manipulators>(this);

	// Setup scenes
	m_mainScene = std::make_shared<MainScene>(this);
	m_previewScene = std::make_shared<PreviewScene>(this);

	m_mainScene->init();
	m_previewScene->init();

	// Setup scene render targets (some are lazy initialized)
	RenderOptions previewOptions;
	previewOptions.framebufferAlpha = true;

	previewRenderTarget = m_previewScene->createRenderTarget(previewOptions);
}

WPtr<Framebuffer> Viewport::drawViewport(int width, int height, const RenderOptions& renderOptions,
                                         const DisplayOptions& displayOptions)
{
	if (!viewportRenderTarget)
	{
		viewportRenderTarget = m_mainScene->createRenderTarget(renderOptions);
	}
	else
	{
		viewportRenderTarget->setRenderOptions(renderOptions);
	}
	m_mainScene->draw(width, height, *viewportRenderTarget, displayOptions);
	return viewportRenderTarget->getOutputFramebuffer();
}

WPtr<Framebuffer> Viewport::drawScreen(int width, int height, glm::mat4 view, glm::mat4 projection,
                                       const RenderOptions& renderOptions, const DisplayOptions& displayOptions)
{
	if (!screenRenderTarget)
	{
		screenRenderTarget = m_mainScene->createRenderTarget(renderOptions);
	}
	else
	{
		screenRenderTarget->setRenderOptions(renderOptions);
	}
	m_mainScene->draw(width, height, view, projection, *screenRenderTarget, displayOptions);
	return screenRenderTarget->getOutputFramebuffer();
}

WPtr<Framebuffer> Viewport::drawPreview(int width, int height, std::weak_ptr<GameObject> gameObject)
{
	if (auto gameObjectPtr = gameObject.lock())
	{
		// Set camera distance
		float radius =
		    glm::distance(gameObjectPtr->m_mesh->m_boundingBoxMin, gameObjectPtr->m_mesh->m_boundingBoxMax) / 2.0f;

		m_previewScene->m_orbitCamera->setFov(m_settings.preview_fov);
		m_previewScene->m_orbitCamera->setRadius(radius * m_settings.preview_radiusFactor);

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
		m_previewScene->draw(width, height, *previewRenderTarget, displayOptions);
		m_previewScene->m_entities.pop_back();
		gameObjectPtr->m_modelMatrix = temp;

		// Restore object visibility
		gameObjectPtr->m_visible = visibleTemp;

		return previewRenderTarget->getOutputFramebuffer();
	}
	else
	{
		LOG_ERROR("Viewport: drawPreview(): Passed game object is NULL!");
		return WPtr<Framebuffer>();
	}
}

void Viewport::update(double dt)
{
	m_mainScene->update(dt);

	m_previewScene->m_orbitCamera->setRotationX(m_previewScene->m_orbitCamera->getRotationX() +
	                                            m_settings.preview_rotateSpeed * dt);
}

void Viewport::processInput(double dt, glm::vec2 mousePos, glm::ivec2 windowSize)
{
	m_mainScene->processInput(dt, mousePos, windowSize);
}

void Viewport::processSelection(glm::vec2 mousePos, glm::ivec2 windowSize)
{
	m_mainScene->processSelection(*viewportRenderTarget, mousePos, windowSize);
}

std::weak_ptr<SceneModel> Viewport::createModel(Core::ID guiNodeId)
{
	Core::Mesh* mesh = RM::instance().mesh("default_cube", "Data/Models/CubeFixed.gltf");
	auto sceneModel = std::make_shared<SceneModel>("default_cube", Shaders::instance().m_phongShader.get());
	sceneModel->m_guiNodeId = guiNodeId;
	m_mainScene->addEntity(sceneModel);
	return sceneModel;
}

std::weak_ptr<SceneCamera> Viewport::createCamera(Core::ID guiNodeId)
{
	// TODO: (DR) It'd be nice to use a camera model with a lens assembly looking similar to a frustum
	//  Key word being an old projector or some old grain film movie cameras.
	Core::Mesh* mesh = RM::instance().mesh("Data/Models/super8.gltf");
	auto sceneCamera = std::make_shared<SceneCamera>(mesh, Shaders::instance().m_phongShader.get());
	sceneCamera->m_guiNodeId = guiNodeId;
	m_mainScene->addEntity(sceneCamera);
	return sceneCamera;
}

// TODO: (DR) Should probably be something like viewport->getMainScene()->getCamera() since its not clear which camera
//   we're actually getting here (without reading the doc)
std::weak_ptr<AggregateCamera> Viewport::getMainViewportCamera()
{
	std::shared_ptr<AggregateCamera> camera = std::dynamic_pointer_cast<AggregateCamera>(m_mainScene->m_camera);
	return camera;
}

ViewportSettings& Viewport::getSettings() { return m_settings; }

Manipulators& Viewport::getManipulators() { return *m_manipulators; };
