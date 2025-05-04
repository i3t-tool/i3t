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
#include "TrackballCamera.h"

using namespace Vp;

#include <glm/gtc/matrix_transform.hpp>

#include "Core/Input/InputManager.h"
#include "Trackball.h"

// For debug purposes
#include "GUI/Toolkit.h"
#include "I3T.h"

void TrackballCamera::update()
{
	m_projection = createProjectionMatrix(true);

	m_view = glm::lookAt(m_pivot, m_pivot + glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0));
	m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_radius)) * m_accumulatedRotation * m_view;
	glm::mat4 viewInv = glm::inverse(m_view);

	m_right = -glm::vec3(viewInv[0]);
	m_up = glm::vec3(viewInv[1]);
	m_direction = -glm::vec3(viewInv[2]);
	m_position = glm::vec3(viewInv[3]);
}

void TrackballCamera::processInput(double dt, glm::vec2 mousePos, glm::ivec2 windowSize)
{
	float dx = InputManager::m_mouseXDelta;
	float dy = InputManager::m_mouseYDelta;

	bool rotate = InputManager::isAxisActive("rotate");
	bool pan = InputManager::isAxisActive("pan");

	// Rotate
	if (rotate)
	{
		glm::mat4 screenSpaceRotation = Trackball::rotate(mousePos, mousePos + glm::vec2(dx, dy), windowSize);
		m_accumulatedRotation = screenSpaceRotation * m_accumulatedRotation;

		// Debug only
		if (I3T::app().m_debugTrackball)
		{
			glm::vec2 windowPos = I3T::getUI()->getWindowManager().getCurrentWindow()->m_windowPos;
			glm::vec2 windowCenter = windowPos + glm::vec2(windowSize.x / 2, windowSize.y / 2);
			// This debug draw shows exact area what projectToSphere() considers inside and outside of the sphere.
			GUI::drawEllipse(windowCenter.x, windowCenter.y, Trackball::debug_trackballScreenSize.x,
			                 Trackball::debug_trackballScreenSize.y, 32, ImGui::GetForegroundDrawList(),
			                 ImColor(1.f, 0.f, 0.f, 1.f));
		}
	}

	// TODO: (DR) Make pan speed dependent on window size
	// TODO: (DR) Idea: Implement perfect panning on world grid? https://prideout.net/blog/perfect_panning/
	// Pan
	if (pan)
	{
		glm::vec3 oldPivot = glm::vec3(m_pivot);

		const float ratio = m_radius / m_zNear / 100.0f;
		m_pivot += glm::vec3(m_right) * (m_translateSpeed * dx * ratio);
		m_pivot += glm::vec3(m_up) * (m_translateSpeed * dy * ratio);
	}

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

void TrackballCamera::mouseWheel(float scroll)
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

void TrackballCamera::viewpoint(AbstractCamera::Viewpoint viewpoint)
{
	glm::mat4 from = m_view;
	switch (viewpoint)
	{
	case AbstractCamera::Viewpoint::RIGHT:
		setRotation(glm::mat4(1));
		break;
	case AbstractCamera::Viewpoint::LEFT:
		this->setRotation(glm::mat4({-1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, -1, 0}, {0, 0, 0, 1}));
		break;
	case AbstractCamera::Viewpoint::TOP:
		this->setRotation(glm::mat4({0, 1, 0, 0}, {0, 0, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 1}));
		break;
	case AbstractCamera::Viewpoint::BOTTOM:
		this->setRotation(glm::mat4({0, -1, 0, 0}, {0, 0, -1, 0}, {1, 0, 0, 0}, {0, 0, 0, 1}));
		break;
	case AbstractCamera::Viewpoint::FRONT:
		this->setRotation(glm::mat4({0, 0, -1, 0}, {0, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 1}));
		break;
	case AbstractCamera::Viewpoint::BACK:
		this->setRotation(glm::mat4({0, 0, 1, 0}, {0, 1, 0, 0}, {-1, 0, 0, 0}, {0, 0, 0, 1}));
		break;
	}
	update();
	interpolate(from, m_view);
}

glm::mat4 TrackballCamera::getRotation() const
{
	return m_accumulatedRotation;
}

void TrackballCamera::setRotation(glm::mat4 rotation)
{
	m_accumulatedRotation = rotation;
}

float TrackballCamera::getZoomSpeed() const
{
	return m_zoomSpeed;
}
void TrackballCamera::setZoomSpeed(float zoomSpeed)
{
	m_zoomSpeed = zoomSpeed;
}
float TrackballCamera::getRotateSpeed() const
{
	return m_rotateSpeed;
}
void TrackballCamera::setRotateSpeed(float rotateSpeed)
{
	m_rotateSpeed = rotateSpeed;
}
float TrackballCamera::getTranslateSpeed() const
{
	return m_translateSpeed;
}
void TrackballCamera::setTranslateSpeed(float translateSpeed)
{
	m_translateSpeed = translateSpeed;
}
bool TrackballCamera::getSmoothScroll() const
{
	return m_smoothScroll;
}
void TrackballCamera::setSmoothScroll(bool b)
{
	m_smoothScroll = b;
}
float TrackballCamera::getSmoothScrollDamping() const
{
	return m_smoothScrollDamping;
}
void TrackballCamera::setSmoothScrollDamping(float val)
{
	m_smoothScrollDamping = val;
}
