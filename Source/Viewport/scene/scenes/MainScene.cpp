#include "MainScene.h"

#include <random>

#include "Core/Resources/ResourceManager.h"

#include "Utils/Color.h"

#include "Viewport/Shaper.h"
#include "Viewport/Viewport.h"
#include "Viewport/entity/ColoredObject.h"
#include "Viewport/scene/Lighting.h"
#include "Viewport/scene/lights/SunLight.h"
#include "Viewport/shader/ColorShader.h"
#include "Viewport/shader/GridShader.h"
#include "Viewport/shader/PhongShader.h"

using namespace Vp;

MainScene::MainScene(Viewport* viewport) : Scene(viewport)
{
	m_sun1_dir = glm::vec3(-0.73, -0.64, -0.21);
	m_sun2_dir = glm::vec3(0.76, 0.58, -0.12);

	m_sun2_intensity = 0.08f;
}

std::default_random_engine& randomEngine();

inline int generateInt(int from = 0, int to = 3)
{
	std::mt19937 mt{std::random_device{}()};
	std::uniform_int_distribution<> dist(from, to);

	return static_cast<int>(dist(mt));
}

inline float generateFloat(float from = -10.0f, float to = 10.0f)
{
	std::mt19937 mt{std::random_device{}()};
	std::uniform_real_distribution<> dist(from, to);

	return static_cast<float>(dist(mt));
}

void MainScene::init()
{
	Scene::init();

	// TODO: (DR) Some of these settings should be updated dynamically, not on init only
	// Camera
	if (auto camera = std::dynamic_pointer_cast<AggregateCamera>(m_camera))
	{
		camera->switchMode(m_viewport->m_settings.mainScene_cameraMode);
		camera->getOrbitCamera()->setRotationX(m_viewport->m_settings.mainScene_orbitCameraRotationX);
		camera->getOrbitCamera()->setRotationY(m_viewport->m_settings.mainScene_orbitCameraRotationY);

		camera->getOrbitCamera()->setRadius(m_viewport->m_settings.mainScene_iorbitCameraRadius);
		camera->getTrackballCamera()->setRadius(m_viewport->m_settings.mainScene_iorbitCameraRadius);

		camera->getOrbitCamera()->setPivot(m_viewport->m_settings.mainScene_iorbitCameraPivot);
		camera->getTrackballCamera()->setPivot(m_viewport->m_settings.mainScene_iorbitCameraPivot);

		camera->getOrbitCamera()->setSmoothScroll(m_viewport->m_settings.camera_smoothScroll);
		camera->getTrackballCamera()->setSmoothScroll(m_viewport->m_settings.camera_smoothScroll);

		camera->getOrbitCamera()->setFov(m_viewport->m_settings.camera_fov);
		camera->getTrackballCamera()->setFov(m_viewport->m_settings.camera_fov);

		camera->getOrbitCamera()->setZNear(m_viewport->m_settings.camera_zNear);
		camera->getTrackballCamera()->setZNear(m_viewport->m_settings.camera_zNear);

		camera->getOrbitCamera()->setZFar(m_viewport->m_settings.camera_zFar);
		camera->getTrackballCamera()->setZFar(m_viewport->m_settings.camera_zFar);

		camera->getOrbitCamera()->setRotateSpeed(m_viewport->m_settings.orbitCamera_rotateSpeed);
		camera->getOrbitCamera()->setTranslateSpeed(m_viewport->m_settings.orbitCamera_translateSpeed);
		camera->getOrbitCamera()->setZoomSpeed(m_viewport->m_settings.orbitCamera_zoomSpeed);

		camera->getTrackballCamera()->setRotateSpeed(m_viewport->m_settings.trackballCamera_rotateSpeed);
		camera->getTrackballCamera()->setTranslateSpeed(m_viewport->m_settings.trackballCamera_translateSpeed);
		camera->getTrackballCamera()->setZoomSpeed(m_viewport->m_settings.trackballCamera_zoomSpeed);
	}

	// Lights
	m_sun1 = new SunLight();
	m_sun1->intensity = 0.8f;
	m_sun1->color = glm::vec3(0.93, 0.98, 1.0);
	m_sun1->direction = m_sun1_dir;
	m_sun1->pos = glm::vec3(0, 4, 0);
	m_lighting->addLight(m_sun1);

	m_sun2 = new SunLight();
	m_sun2->intensity = m_sun2_intensity;
	m_sun2->color = glm::vec3(0.804, 0.945, 1);
	m_sun2->direction = m_sun2_dir;
	m_sun2->pos = glm::vec3(0, 2, 0);
	m_lighting->addLight(m_sun2);

	m_gridObject =
	    std::make_shared<GameObject>(RMI.meshByAlias(Shaper::screenQuad), Shaders::instance().m_gridShader.get());
	m_gridObject->m_opaque = false;
	m_gridObject->m_explicitTransparencyOrder = 1;
	m_gridObject->setDisplayType(DisplayType::Grid);
	addEntity(m_gridObject);

	//  // Test: Drawing grid using shaper
	//	Shaper shaper;
	//	glm::vec3 pos = glm::vec3(0, 0, 0);
	//	int count = 1000;
	//	glm::vec3 color = glm::vec3(0.45, 0.49, 0.53);
	//	for (int i = 0; i < count; i++)
	//	{
	//		shaper.line(pos + glm::vec3(i, 0.0f, 0.0f), pos + glm::vec3(i, 0.0f, count), color);
	//	}
	//	for (int i = 0; i < count; i++)
	//	{
	//		shaper.line(pos + glm::vec3(0.0f, 0.0f, i), pos + glm::vec3(count, 0.0f, i), color);
	//	}
	//	shaper.createLineMesh("grido");
	//	std::shared_ptr<ColoredObject> obj =
	//	    std::make_shared<ColoredObject>(RMI.meshByAlias("grido"), Shaders::instance().m_colorShader.get());
	//	addEntity(obj);

	//	// Transparency test
	//	Ptr<ColoredObject> a =
	//	    std::make_shared<ColoredObject>(RMI.meshByAlias(Shaper::plane), Shaders::instance().m_colorShader.get());
	//	a->m_opaque = false;
	//	a->m_opacity = 0.6f;
	//	a->m_explicitTransparencyOrder = 1;
	//	a->setDisplayType(DisplayType::Axes);
	//	a->m_modelMatrix = glm::translate(a->m_modelMatrix, glm::vec3(0.5f, 0.5f, 0.0f));
	//	a->m_modelMatrix = glm::scale(a->m_modelMatrix, glm::vec3(100.0f));
	//	a->setColor(glm::vec3(1.0, 0.0, 0.0));
	//	addEntity(a);
	//
	//	Ptr<ColoredObject> b =
	//	    std::make_shared<ColoredObject>(RMI.meshByAlias(Shaper::plane), Shaders::instance().m_colorShader.get());
	//	b->m_opaque = false;
	//	b->m_opacity = 0.3f;
	//	b->m_explicitTransparencyOrder = 2;
	//	b->setDisplayType(DisplayType::Axes);
	//	b->m_modelMatrix = glm::translate(b->m_modelMatrix, glm::vec3(0.5f, 0.5f, 0.0f));
	//	b->m_modelMatrix = glm::rotate(b->m_modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//	b->m_modelMatrix = glm::scale(b->m_modelMatrix, glm::vec3(100.0f));
	//	b->setColor(glm::vec3(0.0, 0.0, 1.0));
	//	addEntity(b);
	//
	//	// Cube array
	//	float step = 2.7f;
	//	for (int y = 0; y < 10; y++)
	//	{
	//		for (int x = 0; x < 10; x++)
	//		{
	//			Ptr<ColoredObject> cube = std::make_shared<ColoredObject>(RMI.meshByAlias(Shaper::unitCube),
	//			                                                          Shaders::instance().m_colorShader.get());
	//			cube->m_opaque = false;
	//			cube->m_opacity = generateFloat(0.2f, 0.6f);
	//			cube->m_modelMatrix = glm::translate(cube->m_modelMatrix, glm::vec3(step * x, 0.0f, step * y));
	//			cube->m_modelMatrix = glm::scale(cube->m_modelMatrix, glm::vec3(1.0f, generateFloat(1, 3), 1.0f));
	//			cube->m_modelMatrix = glm::translate(cube->m_modelMatrix, glm::vec3(0, 0.5f, 0));
	//			cube->setColor(glm::vec3(generateFloat(0, 1), generateFloat(0, 1), generateFloat(0, 1)));
	//			addEntity(cube);
	//		}
	//	}
}

void MainScene::draw(int width, int height, SceneRenderTarget& renderTarget, const DisplayOptions& displayOptions)
{
	m_camera->size(width, height);
	m_camera->update();

	return draw(width, height, m_camera->getView(), m_camera->getProjection(), renderTarget, displayOptions);
}

void MainScene::draw(int width, int height, glm::mat4 view, glm::mat4 projection, SceneRenderTarget& renderTarget,
                     const DisplayOptions& displayOptions)
{
	GridShader* gridShader = static_cast<GridShader*>(m_gridObject->m_shader);
	gridShader->m_showGrid = displayOptions.showGridLines;
	gridShader->m_showXAxis = displayOptions.showGridXAxis;
	gridShader->m_showYAxis = displayOptions.showGridYAxis;
	gridShader->m_showZAxis = displayOptions.showGridZAxis;
	m_gridObject->m_visible = displayOptions.showGridLines || displayOptions.showGridXAxis ||
	                          displayOptions.showGridYAxis || displayOptions.showGridZAxis;

	Scene::draw(width, height, view, projection, renderTarget, displayOptions);
}

void MainScene::update(double dt)
{
	Scene::update(dt);

	GridShader* gridShader = static_cast<GridShader*>(m_gridObject->m_shader);
	gridShader->m_gridColor = m_viewport->getSettings().grid_color;
	gridShader->m_axisXColor = m_viewport->getSettings().grid_axisXColor;
	gridShader->m_axisYColor = m_viewport->getSettings().grid_axisYColor;
	gridShader->m_axisZColor = m_viewport->getSettings().grid_axisZColor;

	gridShader->m_gridSize = m_viewport->getSettings().grid_size;
	gridShader->m_gridStrength = m_viewport->getSettings().grid_strength;
	gridShader->m_lineWidth = m_viewport->getSettings().grid_lineWidth;

	gridShader->m_grid1FadeStart = m_viewport->getSettings().grid_grid1FadeStart;
	gridShader->m_grid1FadeEnd = m_viewport->getSettings().grid_grid1FadeEnd;
	gridShader->m_grid2FadeStart = m_viewport->getSettings().grid_grid2FadeStart;
	gridShader->m_grid2FadeEnd = m_viewport->getSettings().grid_grid2FadeEnd;

	if (m_viewport->getSettings().mainScene_lightFollowsCamera)
	{
		m_sun1->direction = glm::inverse(m_camera->getView()) * glm::normalize(glm::vec4(-0.3f, -0.4f, -1.0f, 0.0f));
		m_sun2->intensity = 0.0f;
	}
	else
	{
		m_sun1->direction = m_sun1_dir;
		m_sun2->intensity = m_sun2_intensity;
	}
}
