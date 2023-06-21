#include "AggregateCamera.h"

using namespace Vp;

#include <cmath>

#include "glm/gtc/matrix_transform.hpp"

#include "Logger/Logger.h"

AggregateCamera::AggregateCamera()
{
	m_orbitCamera = std::make_shared<OrbitCamera>();
	m_trackballCamera = std::make_shared<TrackballCamera>();

	switchMode(CameraMode::TRACKBALL);
}

void AggregateCamera::switchMode(CameraMode newMode)
{
	if (m_activeMode == newMode || newMode == CameraMode::NONE)
		return;

	CameraMode prevMode = m_activeMode;

	// Switch mode
	switch (newMode)
	{
	case CameraMode::ORBIT:
		m_activeCamera = m_orbitCamera;
		m_activeMode = newMode;
		break;
	case CameraMode::TRACKBALL:
		m_activeCamera = m_trackballCamera;
		m_activeMode = newMode;
		break;
	}

	// Setup new mode based on previous mode state
	if (prevMode == CameraMode::NONE)
		return;

	if (prevMode == CameraMode::ORBIT && newMode == CameraMode::TRACKBALL)
	{
		// Construct trackball camera's rotation matrix with an azimuth and elevation rotation
		float rotX = -m_orbitCamera->getRotationX();
		float rotY = m_orbitCamera->getRotationY();
		glm::mat4 rotation = glm::mat4(1);
		rotation = glm::rotate(rotation, glm::radians(rotY), glm::vec3(1, 0, 0));
		rotation = glm::rotate(rotation, glm::radians(rotX), glm::vec3(0, 1, 0));
		m_trackballCamera->setRotation(rotation);

		m_trackballCamera->setPivot(m_orbitCamera->getPivot());
		m_trackballCamera->setRadius(m_orbitCamera->getRadius());

		m_trackballCamera->setFov(m_orbitCamera->getFov());
		m_trackballCamera->setZNear(m_orbitCamera->getZNear());
		m_trackballCamera->setZFar(m_orbitCamera->getZFar());
	}
	if (prevMode == CameraMode::TRACKBALL && newMode == CameraMode::ORBIT)
	{
		// TODO: (DR) Orbit camera will always be upright, so if the trackball camera is upside down it does a barrel
		// roll

		// Calculate azimuth and elevation from trackball's camera direction vector
		// A conversion from cartesian to spherical coordinates
		glm::vec3 dir = m_trackballCamera->getDirection();
		float angleX = -glm::degrees(glm::atan(dir.z, dir.x));
		angleX = -glm::sign(angleX) * (180 - glm::abs(angleX)); // Adjust range
		float angleY = -glm::degrees(glm::atan(dir.y, (float) hypot(dir.x, dir.z)));
		m_orbitCamera->setRotationX(angleX);
		m_orbitCamera->setRotationY(angleY);

		m_orbitCamera->setPivot(m_trackballCamera->getPivot());
		m_orbitCamera->setRadius(m_trackballCamera->getRadius());

		m_orbitCamera->setFov(m_trackballCamera->getFov());
		m_orbitCamera->setZNear(m_trackballCamera->getZNear());
		m_orbitCamera->setZFar(m_trackballCamera->getZFar());
	}
}

void AggregateCamera::size(int width, int height)
{
	m_activeCamera->size(width, height);
}

void AggregateCamera::update()
{
	m_activeCamera->update();
}

void AggregateCamera::processInput(double dt, glm::vec2 mousePos, glm::ivec2 windowSize)
{
	m_activeCamera->processInput(dt, mousePos, windowSize);
}

AggregateCamera::CameraMode AggregateCamera::getMode() const
{
	return m_activeMode;
}
const std::shared_ptr<AbstractCamera>& AggregateCamera::getActiveCamera() const
{
	return m_activeCamera;
}
const std::shared_ptr<OrbitCamera>& AggregateCamera::getOrbitCamera() const
{
	return m_orbitCamera;
}
const std::shared_ptr<TrackballCamera>& AggregateCamera::getTrackballCamera() const
{
	return m_trackballCamera;
}

void AggregateCamera::viewpoint(AbstractCamera::Viewpoint viewpoint)
{
	m_activeCamera->viewpoint(viewpoint);
}

void AggregateCamera::centerOnScene(const Scene& scene)
{
	m_activeCamera->centerOnScene(scene);
}
void AggregateCamera::centerOnSelection(const Scene& scene)
{
	m_activeCamera->centerOnSelection(scene);
}
void AggregateCamera::centerOnObjects(const std::vector<const GameObject*> objects)
{
	m_activeCamera->centerOnObjects(objects);
}
void AggregateCamera::centerOnBox(glm::vec3 boxMin, glm::vec3 boxMax, bool interpolate)
{
	m_activeCamera->centerOnBox(boxMin, boxMax, interpolate);
}

glm::mat4 AggregateCamera::getView() const
{
	return m_activeCamera->getView();
}
glm::mat4 AggregateCamera::getProjection() const
{
	return m_activeCamera->getProjection();
}

float AggregateCamera::getZNear() const
{
	return m_activeCamera->getZNear();
}
void AggregateCamera::setZNear(float zNear)
{
	m_activeCamera->setZNear(zNear);
}
float AggregateCamera::getZFar() const
{
	return m_activeCamera->getZFar();
}
void AggregateCamera::setZFar(float zFar)
{
	m_activeCamera->setZFar(zFar);
}
float AggregateCamera::getFov() const
{
	return m_activeCamera->getFov();
}
void AggregateCamera::setFov(float fov)
{
	m_activeCamera->setFov(fov);
}

int AggregateCamera::getWidth() const
{
	return m_activeCamera->getWidth();
}
int AggregateCamera::getHeight() const
{
	return m_activeCamera->getHeight();
}

glm::vec3 AggregateCamera::getPosition() const
{
	return m_activeCamera->getPosition();
}
glm::vec3 AggregateCamera::getDirection() const
{
	return m_activeCamera->getDirection();
}
glm::vec3 AggregateCamera::getUp() const
{
	return m_activeCamera->getUp();
}
glm::vec3 AggregateCamera::getRight() const
{
	return m_activeCamera->getRight();
}
