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
#include "AggregateCamera.h"

#include "Viewport/data/ViewportGlobalSettings.h"

using namespace Vp;

#include <cmath>

#include "glm/gtc/matrix_transform.hpp"

#include "Logger/Logger.h"
#include "Utils/Math.h"
#include "Viewport/data/ViewportSceneSettings.h"

AggregateCamera::AggregateCamera()
{
	m_orbitCamera = std::make_shared<OrbitCamera>();
	m_trackballCamera = std::make_shared<TrackballCamera>();

	switchMode(CameraMode::ORBIT);
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
		// Calculate azimuth and elevation from trackball's camera direction vector
		// A kind of conversion from cartesian to spherical coordinates
		float angleX;
		glm::vec3 dir = m_trackballCamera->getDirection();
		glm::vec3 up = m_trackballCamera->getUp();
		if (Math::eqToZero(dir.z) && Math::eqToZero(dir.x))
		{
			// The camera direction is fully vertical -> determine X rotation using the up vector
			angleX = -glm::sign(dir.y) * glm::degrees(glm::atan(up.z, up.x));
		}
		else
		{
			angleX = -glm::degrees(glm::atan(dir.z, dir.x));
			if (up.y >= 0) // Trackball camera is NOT upside down
				angleX = angleX == 0 ? 180 : -glm::sign(angleX) * (180 - glm::abs(angleX)); // Adjust range
		}
		float angleY = -glm::degrees(glm::atan(dir.y, (float) hypot(dir.x, dir.z)));
		if (up.y < 0) // Trackball camera is upside down
			angleY = glm::sign(angleY) * (180 - abs(angleY));

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

void AggregateCamera::loadSettings(CameraSettings& stg)
{
	switchMode(stg.mode);

	getOrbitCamera()->setRotationX(stg.orbitCameraRotationX);
	getOrbitCamera()->setRotationY(stg.orbitCameraRotationY);
	getTrackballCamera()->setRotation(stg.trackballCameraRotation);

	getOrbitCamera()->setRadius(stg.iorbitCameraRadius);
	getTrackballCamera()->setRadius(stg.iorbitCameraRadius);

	getOrbitCamera()->setPivot(stg.iorbitCameraPivot);
	getTrackballCamera()->setPivot(stg.iorbitCameraPivot);

	getOrbitCamera()->setFov(stg.fov);
	getTrackballCamera()->setFov(stg.fov);

	getOrbitCamera()->setZNear(stg.zNear);
	getTrackballCamera()->setZNear(stg.zNear);

	getOrbitCamera()->setZFar(stg.zFar);
	getTrackballCamera()->setZFar(stg.zFar);
}

void AggregateCamera::saveSettings(CameraSettings& stg)
{
	stg.mode = getMode();

	switch (getMode())
	{
	case AggregateCamera::CameraMode::ORBIT:
		stg.orbitCameraRotationX = getOrbitCamera()->getRotationX();
		stg.orbitCameraRotationY = getOrbitCamera()->getRotationY();

		stg.iorbitCameraRadius = getOrbitCamera()->getRadius();
		stg.iorbitCameraPivot = getOrbitCamera()->getPivot();

		stg.fov = getOrbitCamera()->getFov();
		stg.zNear = getOrbitCamera()->getZNear();
		stg.zFar = getOrbitCamera()->getZFar();

		break;
	case AggregateCamera::CameraMode::TRACKBALL:
		stg.trackballCameraRotation = getTrackballCamera()->getRotation();

		stg.iorbitCameraRadius = getTrackballCamera()->getRadius();
		stg.iorbitCameraPivot = getTrackballCamera()->getPivot();

		stg.fov = getTrackballCamera()->getFov();
		stg.zNear = getTrackballCamera()->getZNear();
		stg.zFar = getTrackballCamera()->getZFar();

		break;
	default:
		break;
	}
}

void AggregateCamera::loadSettings(GlobalCameraSettings& stg)
{
	getOrbitCamera()->setSmoothScroll(stg.smoothScroll);
	getTrackballCamera()->setSmoothScroll(stg.smoothScroll);
	getOrbitCamera()->setSmoothScrollDamping(stg.smoothScrollDamping);
	getTrackballCamera()->setSmoothScrollDamping(stg.smoothScrollDamping);

	getOrbitCamera()->setRotateSpeed(stg.orbit_rotateSpeed);
	getOrbitCamera()->setTranslateSpeed(stg.orbit_translateSpeed);
	getOrbitCamera()->setZoomSpeed(stg.orbit_zoomSpeed);

	getTrackballCamera()->setRotateSpeed(stg.trackball_rotateSpeed);
	getTrackballCamera()->setTranslateSpeed(stg.trackball_translateSpeed);
	getTrackballCamera()->setZoomSpeed(stg.trackball_zoomSpeed);
}

void AggregateCamera::saveSettings(GlobalCameraSettings& stg)
{
	switch (getMode())
	{
	case AggregateCamera::CameraMode::ORBIT:
		stg.smoothScroll = getOrbitCamera()->getSmoothScroll();
		stg.smoothScrollDamping = getOrbitCamera()->getSmoothScrollDamping();
		stg.orbit_rotateSpeed = getOrbitCamera()->getRotateSpeed();
		stg.orbit_translateSpeed = getOrbitCamera()->getTranslateSpeed();
		stg.orbit_zoomSpeed = getOrbitCamera()->getZoomSpeed();
		break;
	case AggregateCamera::CameraMode::TRACKBALL:
		stg.smoothScroll = getTrackballCamera()->getSmoothScroll();
		stg.smoothScrollDamping = getTrackballCamera()->getSmoothScrollDamping();
		stg.trackball_rotateSpeed = getTrackballCamera()->getRotateSpeed();
		stg.trackball_translateSpeed = getTrackballCamera()->getTranslateSpeed();
		stg.trackball_zoomSpeed = getTrackballCamera()->getZoomSpeed();
		break;
	default:
		break;
	}
}