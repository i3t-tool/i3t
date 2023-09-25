#include "PreviewScene.h"

#include "Viewport/camera/OrbitCamera.h"
#include "Viewport/scene/lights/SunLight.h"

using namespace Vp;

#define PREVIEW_FOV 60
#define PREVIEW_CAMERA_X 50.0f

PreviewScene::PreviewScene(Viewport* viewport) : Scene(viewport)
{
	m_orbitCamera = std::make_shared<OrbitCamera>();
	m_camera = m_orbitCamera;
}

void PreviewScene::init()
{
	Scene::init();

	m_orbitCamera->setFov(PREVIEW_FOV);
	m_orbitCamera->setRotationX(PREVIEW_CAMERA_X);

	// Lights
	SunLight* sun = new SunLight();
	sun->intensity = 0.8f;
	sun->color = glm::vec3(0.93, 0.98, 1.0);
	sun->direction = glm::vec3(-0.73, -0.64, -0.21);
	sun->pos = glm::vec3(0, 4, 0);
	m_lighting->addLight(sun);

	SunLight* sun2 = new SunLight();
	sun2->intensity = 0.08f;
	sun2->color = glm::vec3(0.804, 0.945, 1);
	sun2->specular = glm::vec3(0.0f); // Disable specular
	sun2->direction = glm::vec3(0.76, 0.58, 0.19);
	sun->pos = glm::vec3(0, 2, 0);
	m_lighting->addLight(sun2);
}