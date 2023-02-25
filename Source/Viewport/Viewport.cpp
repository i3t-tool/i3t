#include "Viewport.h"

#include "imgui.h"

#include "Core/Resources/ResourceManager.h"
#include "Logger/Logger.h"
#include "Utils/Color.h"

#include "Viewport/Shaper.h"
#include "Viewport/entity/GameObject.h"
#include "Viewport/entity/SceneCamera.h"
#include "Viewport/entity/SceneModel.h"
#include "Viewport/scene/Lighting.h"
#include "Viewport/scene/SceneRenderTarget.h"
#include "Viewport/scene/lights/SunLight.h"
#include "Viewport/shader/ColorShader.h"
#include "Viewport/shader/FrustumShader.h"
#include "Viewport/shader/GridShader.h"
#include "Viewport/shader/PhongShader.h"
#include "Viewport/shader/WBOITCompositeShader.h"

using namespace Vp;

#define PREVIEW_RADIUS_FACTOR 1.72f
#define PREVIEW_ROTATE_SPEED 40.0f
#define PREVIEW_FOV 60
#define PREVIEW_CAMERA_X 50.0f

Viewport::Viewport()
{
	// Empty
}

Viewport::~Viewport() = default;

void Viewport::init()
{
	// Load shaders
	m_shaders = std::make_unique<Shaders>();
	m_shaders->create();

	// Load shapes
	Shaper::initDefaultShapes();

	// Preload some useful models
	RMI.mesh("Data/Models/super8.gltf");
	RMI.mesh("Data/Models/CubeFixed.gltf");

	// Setup scenes
	m_mainScene = std::make_unique<MainScene>(this);
	m_previewScene = std::make_unique<Scene>(this);

	m_mainScene->init();
	initPreviewScene(*m_previewScene);

	// Setup scene render targets
	RenderOptions previewOptions;
	previewOptions.framebufferAlpha = true;

	viewportRenderTarget = m_mainScene->createRenderTarget(RenderOptions());
	screenRenderTarget = m_mainScene->createRenderTarget(RenderOptions());
	previewRenderTarget = m_previewScene->createRenderTarget(previewOptions);
}

WPtr<Framebuffer> Viewport::drawViewport(int width, int height, const RenderOptions& renderOptions,
                                         const DisplayOptions& displayOptions)
{
	viewportRenderTarget->setRenderOptions(renderOptions);
	m_mainScene->draw(width, height, *viewportRenderTarget, displayOptions);
	return viewportRenderTarget->getOutputFramebuffer();
}

WPtr<Framebuffer> Viewport::drawScreen(int width, int height, glm::mat4 view, glm::mat4 projection,
                                       const RenderOptions& renderOptions, const DisplayOptions& displayOptions)
{
	screenRenderTarget->setRenderOptions(renderOptions);
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
		m_previewScene->m_camera->setRadius(radius * PREVIEW_RADIUS_FACTOR);

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

void Viewport::update()
{
	float dt = ImGui::GetIO().DeltaTime;

	m_mainScene->update();

	m_previewScene->m_camera->setRotationX(m_previewScene->m_camera->getRotationX() + PREVIEW_ROTATE_SPEED * dt);
}

void Viewport::processInput(glm::vec2 mousePos, glm::ivec2 windowSize)
{
	m_mainScene->processInput(mousePos, windowSize);
}

std::weak_ptr<SceneModel> Viewport::createModel()
{
	Core::Mesh* mesh = RM::instance().mesh("Data/Models/CubeFixed.gltf");
	auto sceneModel = std::make_shared<SceneModel>(mesh, m_shaders->m_phongShader.get());
	m_mainScene->addEntity(sceneModel);
	return sceneModel;
}

std::weak_ptr<SceneCamera> Viewport::createCamera()
{
	Core::Mesh* mesh = RM::instance().mesh("Data/Models/super8.gltf");
	auto sceneCamera = std::make_shared<SceneCamera>(mesh, m_shaders->m_phongShader.get());
	m_mainScene->addEntity(sceneCamera);
	return sceneCamera;
}

void Viewport::initPreviewScene(Scene& scene)
{
	scene.m_camera->setFov(PREVIEW_FOV);
	scene.m_camera->setRotationX(PREVIEW_CAMERA_X);

	// Lights
	SunLight* sun = new SunLight();
	sun->intensity = 0.8f;
	sun->color = glm::vec3(0.93, 0.98, 1.0);
	sun->direction = glm::vec3(-0.73, -0.64, -0.21);
	sun->pos = glm::vec3(0, 4, 0);
	scene.m_lighting->addLight(sun);

	SunLight* sun2 = new SunLight();
	sun2->intensity = 0.12f;
	sun2->color = glm::vec3(0.69f, 0.91f, 1.0f);
	sun2->direction = glm::vec3(0.76, 0.58, -0.12);
	sun->pos = glm::vec3(0, 2, 0);
	scene.m_lighting->addLight(sun2);
}
