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
#include "Viewport/entity/GridObject.h"
#include "Viewport/scene/Lighting.h"
#include "Viewport/scene/lights/SunLight.h"
#include "Viewport/shader/GridShader.h"

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

	m_worldGrid = std::make_shared<GridObject>();
	m_worldGrid->m_opaque = false;
	m_worldGrid->m_explicitTransparencyOrder = 1;
	addEntity(m_worldGrid);

	m_localGrid = std::make_shared<GridObject>();
	m_localGrid->m_opaque = false;
	m_localGrid->m_explicitTransparencyOrder = 1;
	addEntity(m_localGrid);

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
	//	    std::make_shared<ColoredObject>(RMI.meshByAlias("grido"), SHADERS.getShaderPtr<ColorShader>());
	//	addEntity(obj);

	//	// Transparency test
	//	Ptr<ColoredObject> a =
	//	    std::make_shared<ColoredObject>(RMI.meshByAlias(Shaper::plane), SHADERS.getShaderPtr<ColorShader>());
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
	//	    std::make_shared<ColoredObject>(RMI.meshByAlias(Shaper::plane), SHADERS.getShaderPtr<ColorShader>());
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
	//			                                                          SHADERS.getShaderPtr<ColorShader>());
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

void MainScene::draw(int width, int height, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
                     SceneRenderTarget& renderTarget, const DisplayOptions& displayOptions)
{
	ViewportSettings& stg = m_viewport->getSettings();

	m_worldGrid->m_visible = displayOptions.grid.show && stg.global().grid.programShow;
	m_worldGrid->m_showGrid = displayOptions.grid.showGridLines;
	m_worldGrid->m_showXAxis = displayOptions.grid.showGridXAxis;
	m_worldGrid->m_showYAxis = displayOptions.grid.showGridYAxis;
	m_worldGrid->m_showZAxis = displayOptions.grid.showGridZAxis;
	if (m_worldGrid->m_visible)
	{
		m_worldGrid->m_visible = displayOptions.grid.showGridLines || displayOptions.grid.showGridXAxis ||
		                         displayOptions.grid.showGridYAxis || displayOptions.grid.showGridZAxis;
	}

	m_localGrid->m_visible = displayOptions.localGrid.show && stg.global().localGrid.programShow;
	m_localGrid->m_showGrid = displayOptions.localGrid.showGridLines;
	m_localGrid->m_showXAxis = displayOptions.localGrid.showGridXAxis;
	m_localGrid->m_showYAxis = displayOptions.localGrid.showGridYAxis;
	m_localGrid->m_showZAxis = displayOptions.localGrid.showGridZAxis;
	if (m_localGrid->m_visible)
	{
		m_localGrid->m_visible = displayOptions.localGrid.showGridLines || displayOptions.localGrid.showGridXAxis ||
		                         displayOptions.localGrid.showGridYAxis || displayOptions.localGrid.showGridZAxis;
	}

	// TODO: See comment in GridObject::prepareRenderContext()

	m_worldGrid->m_gridSize = stg.global().grid.size;
	m_worldGrid->m_gridStrength = stg.global().grid.programStrength;
	m_worldGrid->m_lineWidth = stg.global().grid.lineWidth;

	m_worldGrid->m_grid1FadeStart = stg.global().grid.grid1FadeStart;
	m_worldGrid->m_grid1FadeEnd = stg.global().grid.grid1FadeEnd;
	m_worldGrid->m_grid2FadeStart = stg.global().grid.grid2FadeStart;
	m_worldGrid->m_grid2FadeEnd = stg.global().grid.grid2FadeEnd;

	m_localGrid->m_gridSize = stg.global().localGrid.size;
	m_localGrid->m_gridStrength = stg.global().localGrid.programStrength;
	m_localGrid->m_lineWidth = stg.global().localGrid.lineWidth;

	m_localGrid->m_grid1FadeStart = stg.global().localGrid.grid1FadeStart;
	m_localGrid->m_grid1FadeEnd = stg.global().localGrid.grid1FadeEnd;
	m_localGrid->m_grid2FadeStart = stg.global().localGrid.grid2FadeStart;
	m_localGrid->m_grid2FadeEnd = stg.global().localGrid.grid2FadeEnd;

	Scene::draw(width, height, model, view, projection, renderTarget, displayOptions);
}

void MainScene::update(double dt)
{
	Scene::update(dt);

	ViewportSettings& stg = m_viewport->getSettings();

	if (m_lightFollowsCamera)
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
