#include "MainScene.h"

#include "Viewport/Shaper.h"
#include "Viewport/Viewport.h"
#include "Viewport/scene/Lighting.h"
#include "Viewport/scene/lights/SunLight.h"
#include "Viewport/shader/GridShader.h"

#include "Utils/Color.h"

using namespace Vp;

MainScene::MainScene(Viewport* viewport) : Scene(viewport)
{
	// Empty
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

	Shaper s;
	s.quad(glm::vec3(-1, 1, 0), glm::vec3(1, 1, 0), glm::vec3(-1, -1, 0),
	       glm::vec3(1, -1, 0), Color::GREEN);
	Core::Mesh* m = s.createMesh("gridScreenQuad");
	m_gridObject =
	    std::make_shared<GameObject>(m, m_viewport->m_gridShader.get());
	m_gridObject->m_opaque = false;
}

void MainScene::draw(int width, int height) { Scene::draw(width, height); }

void MainScene::draw(glm::mat4 view, glm::mat4 projection,
                     const DisplayOptions& displayOptions)
{
	Scene::draw(view, projection, displayOptions);

	m_gridObject->render(view, projection);
}
