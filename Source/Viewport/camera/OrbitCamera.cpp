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
#include "OrbitCamera.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Core/Input/InputManager.h"

using namespace Vp;

void OrbitCamera::update()
{
	glm::mat4 cameraTransform = glm::mat4(1.0f);
	cameraTransform = glm::translate(cameraTransform, m_pivot);
	cameraTransform = glm::rotate(cameraTransform, glm::radians(m_rotationX), glm::vec3(0.0f, 1.0f, 0.0f));
	cameraTransform = glm::rotate(cameraTransform, glm::radians(m_rotationY), glm::vec3(0.0f, 0.0f, 1.0f));
	cameraTransform = glm::translate(cameraTransform, glm::vec3(m_radius, 0.0f, 0.0f));

	const glm::vec4 cameraPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0);
	const glm::vec4 cameraUp = glm::vec4(0.0f, 1.0f, 0.0f, 0.0);

	const glm::vec3 cameraPosWorld = glm::vec3(cameraTransform * cameraPos);
	const glm::vec3 cameraUpWorld = glm::vec3(cameraTransform * cameraUp);

	m_view = glm::lookAt(cameraPosWorld, m_pivot, cameraUpWorld);

	m_projection = createProjectionMatrix(true);

	m_position = cameraPosWorld;
	m_up = glm::normalize(cameraUpWorld);
	m_direction = glm::normalize(m_pivot - glm::vec3(cameraPosWorld));
	m_right = glm::cross(m_up, m_direction);
}

void OrbitCamera::processInput(double dt, glm::vec2 mousePos, glm::ivec2 windowSize)
{
	float dx = InputManager::m_mouseXDelta;
	float dy = InputManager::m_mouseYDelta;

	// TODO: (DR) Make pan speed dependent on window size
	// TODO: (DR) Idea: Implement perfect panning on world grid? https://prideout.net/blog/perfect_panning/

	mouseDrag(dx, dy, InputManager::isAxisActive("rotate"), InputManager::isAxisActive("pan"));

	// Scroll zoom
	if (InputManager::m_mouseWheelOffset != 0)
	{
		m_dScroll = InputManager::m_mouseWheelOffset;
	}

	mouseWheel(m_dScroll * 144.0f * dt * (m_smoothScroll ? 1 : 3));

	m_dScroll *= pow(m_smoothScrollDamping, 144.0f * dt) * (m_smoothScroll ? 1 : 0);

	if (abs(m_dScroll) < 0.001f)
	{
		m_dScroll = 0.0f;
	}
}

void OrbitCamera::mouseDrag(float dx, float dy, bool rotate, bool pan)
{
	// Rotate
	if (rotate)
	{

		// The code below flips the horizontal rotate direction when the camera
		// turns "upside-down" That is, when vertical rotation is in the range of
		// > 90 and < 270 However in case of a continuous mouse drag the direction
		// isn't flipped until the next drag as to not suddenly change direction
		float rot = fmodf(abs(m_rotationY), 360.0f);
		bool upsideDown = rot > 90.0f && rot < 270.0f;
		if (!m_isRotating)
		{
			// Rotation just started
			m_rotateStartedUpsideDown = upsideDown;
		}
		if (m_rotateStartedUpsideDown)
		{
			dx *= -1;
		}
		setRotationX(m_rotationX + -dx * m_rotateSpeed);
		setRotationY(m_rotationY + dy * m_rotateSpeed);

		m_isRotating = true;
	}
	else
	{
		m_isRotating = false;
	}
	// Pan
	if (pan)
	{
		glm::vec3 oldPivot = glm::vec3(m_pivot);

		const float ratio = m_radius / m_zNear / 100.0f;
		m_pivot += glm::vec3(m_right) * (m_translateSpeed * dx * ratio);
		m_pivot += glm::vec3(m_up) * (m_translateSpeed * dy * ratio);
	}
}

void OrbitCamera::mouseWheel(float scroll)
{
	if (scroll == 0)
		return;

	const float ratio = m_radius / m_zNear / 100.0f;
	m_radius = m_radius - scroll * m_zoomSpeed * ratio;
	if (m_radius < 0.01f)
	{
		m_radius = 0.01f;
	}
}

void OrbitCamera::viewpoint(AbstractCamera::Viewpoint viewpoint)
{
	glm::mat4 from = m_view;
	switch (viewpoint)
	{
	case AbstractCamera::Viewpoint::RIGHT:
		this->setRotationX(0);
		this->setRotationY(0);
		break;
	case AbstractCamera::Viewpoint::LEFT:
		this->setRotationX(180);
		this->setRotationY(0);
		break;
	case AbstractCamera::Viewpoint::TOP:
		this->setRotationX(-90);
		this->setRotationY(90);
		break;
	case AbstractCamera::Viewpoint::BOTTOM:
		this->setRotationX(-90);
		this->setRotationY(-90);
		break;
	case AbstractCamera::Viewpoint::FRONT:
		this->setRotationX(-90);
		this->setRotationY(0);
		break;
	case AbstractCamera::Viewpoint::BACK:
		this->setRotationX(90);
		this->setRotationY(0);
		break;
	}
	update();
	interpolate(from, m_view);
}

float OrbitCamera::getRotationX() const
{
	return m_rotationX;
}
void OrbitCamera::setRotationX(float rotationX)
{
	m_rotationX = fmodf(rotationX, 360.f);
}

float OrbitCamera::getRotationY() const
{
	return m_rotationY;
}
void OrbitCamera::setRotationY(float rotationY)
{
	m_rotationY = fmodf(rotationY, 360.f);
}

float OrbitCamera::getZoomSpeed() const
{
	return m_zoomSpeed;
}
void OrbitCamera::setZoomSpeed(float zoomSpeed)
{
	m_zoomSpeed = zoomSpeed;
}
float OrbitCamera::getRotateSpeed() const
{
	return m_rotateSpeed;
}
void OrbitCamera::setRotateSpeed(float rotateSpeed)
{
	m_rotateSpeed = rotateSpeed;
}
float OrbitCamera::getTranslateSpeed() const
{
	return m_translateSpeed;
}
void OrbitCamera::setTranslateSpeed(float translateSpeed)
{
	m_translateSpeed = translateSpeed;
}
bool OrbitCamera::getSmoothScroll() const
{
	return m_smoothScroll;
}
void OrbitCamera::setSmoothScroll(bool b)
{
	m_smoothScroll = b;
}
float OrbitCamera::getSmoothScrollDamping() const
{
	return m_smoothScrollDamping;
}
void OrbitCamera::setSmoothScrollDamping(float val)
{
	m_smoothScrollDamping = val;
}
