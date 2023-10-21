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
		camera->switchMode(stg.scene().mainScene.camera.mode);
		camera->getOrbitCamera()->setRotationX(stg.scene().mainScene.camera.orbitCameraRotationX);
		camera->getOrbitCamera()->setRotationY(stg.scene().mainScene.camera.orbitCameraRotationY);

		camera->getOrbitCamera()->setRadius(stg.scene().mainScene.camera.iorbitCameraRadius);
		camera->getTrackballCamera()->setRadius(stg.scene().mainScene.camera.iorbitCameraRadius);

		camera->getOrbitCamera()->setPivot(stg.scene().mainScene.camera.iorbitCameraPivot);
		camera->getTrackballCamera()->setPivot(stg.scene().mainScene.camera.iorbitCameraPivot);

		camera->getOrbitCamera()->setSmoothScroll(stg.global().camera.smoothScroll);
		camera->getTrackballCamera()->setSmoothScroll(stg.global().camera.smoothScroll);

		camera->getOrbitCamera()->setFov(stg.scene().mainScene.camera.fov);
		camera->getTrackballCamera()->setFov(stg.scene().mainScene.camera.fov);

		camera->getOrbitCamera()->setZNear(stg.scene().mainScene.camera.zNear);
		camera->getTrackballCamera()->setZNear(stg.scene().mainScene.camera.zNear);

		camera->getOrbitCamera()->setZFar(stg.scene().mainScene.camera.zFar);
		camera->getTrackballCamera()->setZFar(stg.scene().mainScene.camera.zFar);

		camera->getOrbitCamera()->setRotateSpeed(stg.global().camera.orbit_rotateSpeed);
		camera->getOrbitCamera()->setTranslateSpeed(stg.global().camera.orbit_translateSpeed);
		camera->getOrbitCamera()->setZoomSpeed(stg.global().camera.orbit_zoomSpeed);

		camera->getTrackballCamera()->setRotateSpeed(stg.global().camera.trackball_rotateSpeed);
		camera->getTrackballCamera()->setTranslateSpeed(stg.global().camera.trackball_translateSpeed);
		camera->getTrackballCamera()->setZoomSpeed(stg.global().camera.trackball_zoomSpeed);
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
	gridShader->m_gridColor = stg.global().grid.color;
	gridShader->m_axisXColor = stg.global().grid.axisXColor;
	gridShader->m_axisYColor = stg.global().grid.axisYColor;
	gridShader->m_axisZColor = stg.global().grid.axisZColor;

	gridShader->m_gridSize = stg.global().grid.size;
	gridShader->m_gridStrength = stg.global().grid.strength;
	gridShader->m_lineWidth = stg.global().grid.lineWidth;

	gridShader->m_grid1FadeStart = stg.global().grid.grid1FadeStart;
	gridShader->m_grid1FadeEnd = stg.global().grid.grid1FadeEnd;
	gridShader->m_grid2FadeStart = stg.global().grid.grid2FadeStart;
	gridShader->m_grid2FadeEnd = stg.global().grid.grid2FadeEnd;

	if (stg.scene().mainScene.lightFollowsCamera)
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
			camera->switchMode(stg.scene().mainScene.camera.mode);

			camera->getOrbitCamera()->setRotationX(stg.scene().mainScene.camera.orbitCameraRotationX);
			camera->getOrbitCamera()->setRotationY(stg.scene().mainScene.camera.orbitCameraRotationY);
			camera->getTrackballCamera()->setRotation(stg.scene().mainScene.camera.trackballCameraRotation);

			camera->getOrbitCamera()->setRadius(stg.scene().mainScene.camera.iorbitCameraRadius);
			camera->getTrackballCamera()->setRadius(stg.scene().mainScene.camera.iorbitCameraRadius);

			camera->getOrbitCamera()->setPivot(stg.scene().mainScene.camera.iorbitCameraPivot);
			camera->getTrackballCamera()->setPivot(stg.scene().mainScene.camera.iorbitCameraPivot);

			camera->getOrbitCamera()->setFov(stg.scene().mainScene.camera.fov);
			camera->getTrackballCamera()->setFov(stg.scene().mainScene.camera.fov);

			camera->getOrbitCamera()->setZNear(stg.scene().mainScene.camera.zNear);
			camera->getTrackballCamera()->setZNear(stg.scene().mainScene.camera.zNear);

			camera->getOrbitCamera()->setZFar(stg.scene().mainScene.camera.zFar);
			camera->getTrackballCamera()->setZFar(stg.scene().mainScene.camera.zFar);
		}
		if (global)
		{
			camera->getOrbitCamera()->setSmoothScroll(stg.global().camera.smoothScroll);
			camera->getTrackballCamera()->setSmoothScroll(stg.global().camera.smoothScroll);

			camera->getOrbitCamera()->setRotateSpeed(stg.global().camera.orbit_rotateSpeed);
			camera->getOrbitCamera()->setTranslateSpeed(stg.global().camera.orbit_translateSpeed);
			camera->getOrbitCamera()->setZoomSpeed(stg.global().camera.orbit_zoomSpeed);

			camera->getTrackballCamera()->setRotateSpeed(stg.global().camera.trackball_rotateSpeed);
			camera->getTrackballCamera()->setTranslateSpeed(stg.global().camera.trackball_translateSpeed);
			camera->getTrackballCamera()->setZoomSpeed(stg.global().camera.trackball_zoomSpeed);
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
			stg.scene().mainScene.camera.mode = camera->getMode();

		switch (camera->getMode())
		{
		case AggregateCamera::CameraMode::ORBIT:
			if (scene)
			{
				stg.scene().mainScene.camera.orbitCameraRotationX = camera->getOrbitCamera()->getRotationX();
				stg.scene().mainScene.camera.orbitCameraRotationY = camera->getOrbitCamera()->getRotationY();

				stg.scene().mainScene.camera.iorbitCameraRadius = camera->getOrbitCamera()->getRadius();
				stg.scene().mainScene.camera.iorbitCameraPivot = camera->getOrbitCamera()->getPivot();

				stg.scene().mainScene.camera.fov = camera->getOrbitCamera()->getFov();
				stg.scene().mainScene.camera.zNear = camera->getOrbitCamera()->getZNear();
				stg.scene().mainScene.camera.zFar = camera->getOrbitCamera()->getZFar();
			}
			if (global)
			{
				stg.global().camera.smoothScroll = camera->getOrbitCamera()->getSmoothScroll();

				stg.global().camera.orbit_rotateSpeed = camera->getOrbitCamera()->getRotateSpeed();
				stg.global().camera.orbit_translateSpeed = camera->getOrbitCamera()->getTranslateSpeed();
				stg.global().camera.orbit_zoomSpeed = camera->getOrbitCamera()->getZoomSpeed();
			}
			break;
		case AggregateCamera::CameraMode::TRACKBALL:
			if (scene)
			{
				stg.scene().mainScene.camera.trackballCameraRotation = camera->getTrackballCamera()->getRotation();

				stg.scene().mainScene.camera.iorbitCameraRadius = camera->getTrackballCamera()->getRadius();
				stg.scene().mainScene.camera.iorbitCameraPivot = camera->getTrackballCamera()->getPivot();

				stg.scene().mainScene.camera.fov = camera->getTrackballCamera()->getFov();
				stg.scene().mainScene.camera.zNear = camera->getTrackballCamera()->getZNear();
				stg.scene().mainScene.camera.zFar = camera->getTrackballCamera()->getZFar();
			}
			if (global)
			{
				stg.global().camera.smoothScroll = camera->getTrackballCamera()->getSmoothScroll();

				stg.global().camera.trackball_rotateSpeed = camera->getTrackballCamera()->getRotateSpeed();
				stg.global().camera.trackball_translateSpeed = camera->getTrackballCamera()->getTranslateSpeed();
				stg.global().camera.trackball_zoomSpeed = camera->getTrackballCamera()->getZoomSpeed();
			}
			break;
		default:
			break;
		}
	}
}
