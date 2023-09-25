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
	m_sun2_dir = glm::vec3(0.76, 0.58, 0.19);

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

	ViewportSettings& stg = m_viewport->getSettings();

	// TODO: (DR) Some of these settings should be updated dynamically, not on init only
	// Camera
	if (auto camera = std::dynamic_pointer_cast<AggregateCamera>(m_camera))
	{
		camera->switchMode(stg.scene().mainScene_cameraMode);
		camera->getOrbitCamera()->setRotationX(stg.scene().mainScene_orbitCameraRotationX);
		camera->getOrbitCamera()->setRotationY(stg.scene().mainScene_orbitCameraRotationY);

		camera->getOrbitCamera()->setRadius(stg.scene().mainScene_iorbitCameraRadius);
		camera->getTrackballCamera()->setRadius(stg.scene().mainScene_iorbitCameraRadius);

		camera->getOrbitCamera()->setPivot(stg.scene().mainScene_iorbitCameraPivot);
		camera->getTrackballCamera()->setPivot(stg.scene().mainScene_iorbitCameraPivot);

		camera->getOrbitCamera()->setSmoothScroll(stg.global().camera_smoothScroll);
		camera->getTrackballCamera()->setSmoothScroll(stg.global().camera_smoothScroll);

		camera->getOrbitCamera()->setFov(stg.scene().camera_fov);
		camera->getTrackballCamera()->setFov(stg.scene().camera_fov);

		camera->getOrbitCamera()->setZNear(stg.scene().camera_zNear);
		camera->getTrackballCamera()->setZNear(stg.scene().camera_zNear);

		camera->getOrbitCamera()->setZFar(stg.scene().camera_zFar);
		camera->getTrackballCamera()->setZFar(stg.scene().camera_zFar);

		camera->getOrbitCamera()->setRotateSpeed(stg.global().orbitCamera_rotateSpeed);
		camera->getOrbitCamera()->setTranslateSpeed(stg.global().orbitCamera_translateSpeed);
		camera->getOrbitCamera()->setZoomSpeed(stg.global().orbitCamera_zoomSpeed);

		camera->getTrackballCamera()->setRotateSpeed(stg.global().trackballCamera_rotateSpeed);
		camera->getTrackballCamera()->setTranslateSpeed(stg.global().trackballCamera_translateSpeed);
		camera->getTrackballCamera()->setZoomSpeed(stg.global().trackballCamera_zoomSpeed);
	}

	// Lights
	m_sun1 = new SunLight();
	m_sun1->intensity = 1.0f;
	m_sun1->color = glm::vec3(0.93, 0.98, 1.0);
	m_sun1->direction = m_sun1_dir;
	m_sun1->pos = glm::vec3(0, 4, 0);
	m_lighting->addLight(m_sun1);

	m_sun2 = new SunLight();
	m_sun2->intensity = m_sun2_intensity;
	m_sun2->color = glm::vec3(0.804, 0.945, 1);
	m_sun2->specular = glm::vec3(0.0f); // Disable specular
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

	ViewportSettings& stg = m_viewport->getSettings();

	GridShader* gridShader = static_cast<GridShader*>(m_gridObject->m_shader);
	gridShader->m_gridColor = stg.global().grid_color;
	gridShader->m_axisXColor = stg.global().grid_axisXColor;
	gridShader->m_axisYColor = stg.global().grid_axisYColor;
	gridShader->m_axisZColor = stg.global().grid_axisZColor;

	gridShader->m_gridSize = stg.global().grid_size;
	gridShader->m_gridStrength = stg.global().grid_strength;
	gridShader->m_lineWidth = stg.global().grid_lineWidth;

	gridShader->m_grid1FadeStart = stg.global().grid_grid1FadeStart;
	gridShader->m_grid1FadeEnd = stg.global().grid_grid1FadeEnd;
	gridShader->m_grid2FadeStart = stg.global().grid_grid2FadeStart;
	gridShader->m_grid2FadeEnd = stg.global().grid_grid2FadeEnd;

	if (stg.scene().mainScene_lightFollowsCamera)
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

void MainScene::loadSettings(ViewportSettings& stg, bool scene, bool global)
{
	Scene::loadSettings(stg, scene, global);

	// Camera
	if (auto camera = std::dynamic_pointer_cast<AggregateCamera>(m_camera))
	{
		if (scene)
		{
			camera->switchMode(stg.scene().mainScene_cameraMode);

			camera->getOrbitCamera()->setRotationX(stg.scene().mainScene_orbitCameraRotationX);
			camera->getOrbitCamera()->setRotationY(stg.scene().mainScene_orbitCameraRotationY);
			camera->getTrackballCamera()->setRotation(stg.scene().mainScene_trackballCameraRotation);

			camera->getOrbitCamera()->setRadius(stg.scene().mainScene_iorbitCameraRadius);
			camera->getTrackballCamera()->setRadius(stg.scene().mainScene_iorbitCameraRadius);

			camera->getOrbitCamera()->setPivot(stg.scene().mainScene_iorbitCameraPivot);
			camera->getTrackballCamera()->setPivot(stg.scene().mainScene_iorbitCameraPivot);

			camera->getOrbitCamera()->setFov(stg.scene().camera_fov);
			camera->getTrackballCamera()->setFov(stg.scene().camera_fov);

			camera->getOrbitCamera()->setZNear(stg.scene().camera_zNear);
			camera->getTrackballCamera()->setZNear(stg.scene().camera_zNear);

			camera->getOrbitCamera()->setZFar(stg.scene().camera_zFar);
			camera->getTrackballCamera()->setZFar(stg.scene().camera_zFar);
		}
		if (global)
		{
			camera->getOrbitCamera()->setSmoothScroll(stg.global().camera_smoothScroll);
			camera->getTrackballCamera()->setSmoothScroll(stg.global().camera_smoothScroll);

			camera->getOrbitCamera()->setRotateSpeed(stg.global().orbitCamera_rotateSpeed);
			camera->getOrbitCamera()->setTranslateSpeed(stg.global().orbitCamera_translateSpeed);
			camera->getOrbitCamera()->setZoomSpeed(stg.global().orbitCamera_zoomSpeed);

			camera->getTrackballCamera()->setRotateSpeed(stg.global().trackballCamera_rotateSpeed);
			camera->getTrackballCamera()->setTranslateSpeed(stg.global().trackballCamera_translateSpeed);
			camera->getTrackballCamera()->setZoomSpeed(stg.global().trackballCamera_zoomSpeed);
		}
	}
}
void MainScene::saveSettings(ViewportSettings& stg, bool scene, bool global)
{
	Scene::saveSettings(stg, scene, global);

	// Camera
	if (auto camera = std::dynamic_pointer_cast<AggregateCamera>(m_camera))
	{
		if (scene)
			stg.scene().mainScene_cameraMode = camera->getMode();

		switch (camera->getMode())
		{
		case AggregateCamera::CameraMode::ORBIT:
			if (scene)
			{
				stg.scene().mainScene_orbitCameraRotationX = camera->getOrbitCamera()->getRotationX();
				stg.scene().mainScene_orbitCameraRotationY = camera->getOrbitCamera()->getRotationY();

				stg.scene().mainScene_iorbitCameraRadius = camera->getOrbitCamera()->getRadius();
				stg.scene().mainScene_iorbitCameraPivot = camera->getOrbitCamera()->getPivot();

				stg.scene().camera_fov = camera->getOrbitCamera()->getFov();
				stg.scene().camera_zNear = camera->getOrbitCamera()->getZNear();
				stg.scene().camera_zFar = camera->getOrbitCamera()->getZFar();
			}
			if (global)
			{
				stg.global().camera_smoothScroll = camera->getOrbitCamera()->getSmoothScroll();

				stg.global().orbitCamera_rotateSpeed = camera->getOrbitCamera()->getRotateSpeed();
				stg.global().orbitCamera_translateSpeed = camera->getOrbitCamera()->getTranslateSpeed();
				stg.global().orbitCamera_zoomSpeed = camera->getOrbitCamera()->getZoomSpeed();
			}
			break;
		case AggregateCamera::CameraMode::TRACKBALL:
			if (scene)
			{
				stg.scene().mainScene_trackballCameraRotation = camera->getTrackballCamera()->getRotation();

				stg.scene().mainScene_iorbitCameraRadius = camera->getTrackballCamera()->getRadius();
				stg.scene().mainScene_iorbitCameraPivot = camera->getTrackballCamera()->getPivot();

				stg.scene().camera_fov = camera->getTrackballCamera()->getFov();
				stg.scene().camera_zNear = camera->getTrackballCamera()->getZNear();
				stg.scene().camera_zFar = camera->getTrackballCamera()->getZFar();
			}
			if (global)
			{
				stg.global().camera_smoothScroll = camera->getTrackballCamera()->getSmoothScroll();

				stg.global().trackballCamera_rotateSpeed = camera->getTrackballCamera()->getRotateSpeed();
				stg.global().trackballCamera_translateSpeed = camera->getTrackballCamera()->getTranslateSpeed();
				stg.global().trackballCamera_zoomSpeed = camera->getTrackballCamera()->getZoomSpeed();
			}
			break;
		default:
			break;
		}
	}
}
