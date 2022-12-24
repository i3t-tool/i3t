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

using namespace Vp;

MainScene::MainScene(Viewport* viewport) : Scene(viewport)
{
	// Empty
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

	// Lights
	SunLight* sun = new SunLight();
	sun->intensity = 0.8f;
	sun->color = glm::vec3(0.93, 0.98, 1.0);
	sun->direction = glm::vec3(-0.73, -0.64, -0.21);
	sun->pos = glm::vec3(0, 4, 0);
	m_lighting->addLight(sun);

	SunLight* sun2 = new SunLight();
	sun2->intensity = 0.12f;
	sun2->color = glm::vec3(0.69f, 0.91f, 1.0f);
	sun2->direction = glm::vec3(0.76, 0.58, -0.12);
	sun->pos = glm::vec3(0, 2, 0);
	m_lighting->addLight(sun2);

	m_gridObject =
	    std::make_shared<GameObject>(RMI.meshByAlias(Shaper::screenQuad), m_viewport->m_shaders->m_gridShader.get());
	m_gridObject->m_opaque = false;
	m_gridObject->m_explicitTransparencyOrder = 1;
	m_gridObject->setDisplayType(DisplayType::Grid);
	addEntity(m_gridObject);

	//	// Transparency test
	//	Ptr<ColoredObject> a =
	//	    std::make_shared<ColoredObject>(RMI.meshByAlias(Shaper::plane), m_viewport->m_shaders->m_colorShader.get());
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
	//	    std::make_shared<ColoredObject>(RMI.meshByAlias(Shaper::plane), m_viewport->m_shaders->m_colorShader.get());
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
	//			                                                          m_viewport->m_shaders->m_colorShader.get());
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