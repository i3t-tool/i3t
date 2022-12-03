#include <iostream>
#include <typeinfo>

#include "Core/API.h"

#include "../Component.h"
#include "../Transforms.h"
#include "CameraControl.h"
#include "Core/Input/InputBindings.h"
#include "Core/Input/InputManager.h"
#include "GUI/Elements/Windows/ViewportWindow.h"

const char* CameraControl::s_type = NULL;

CameraControl::CameraControl()
{
	CameraControl::s_type = typeid(CameraControl).name();
	this->m_type = CameraControl::s_type;
}
void CameraControl::update()
{
	/// \todo MH
	// if (!InputManager::isFocused<UI::Viewport>())
	if (!InputManager::isInputActive(I3T::getUI()->getWindowPtr<UI::ViewportWindow>()->getInputPtr()))
		return;

	float x = InputManager::m_mouseXDelta / 8.0f;
	float y = InputManager::m_mouseYDelta / 8.0f;

	// if (InputManager::isKeyPressed(Keys::mouseMiddle))
	if (InputManager::isAxisActive("pan"))
	{
		glm::vec4 move = (getRotation(m_gameObject->transformation, 2) * glm::vec4(x, 0.0f, 0.0f, 0.0f) +
		                  glm::vec4(0.0f, y, 0.0f, 0.0f)) *
		                 0.25f;
		if (InputManager::isKeyPressed(Keys::shiftr))
		{
			move *= 3.0f;
		}
		m_gameObject->translate(glm::vec3(-move.x, move.y, -move.z));
	}
	else if (InputManager::isAxisActive("rotate"))
	{
		glm::vec3 center = m_rotateAroundCenter ? glm::vec3(0.0f) : (glm::vec3)m_gameObject->transformation[3];
		// m_gameObject->rotateAround((glm::vec3)m_gameObject->transformation[0],
		// -y, (glm::vec3)m_gameObject->transformation[3]);
		// m_gameObject->rotateAround(glm::vec3(0.0f, 1.0f, 0.0f), -x,
		// 0.0f*(glm::vec3)m_gameObject->transformation[3]);
		m_gameObject->rotateAround((glm::vec3)m_gameObject->transformation[0], -y, center);
		m_gameObject->rotateAround(glm::vec3(0.0f, 1.0f, 0.0f), -x, center);
	}

	glm::vec4 move = glm::vec4(0.0f);
	bool moved = false;
	if (InputManager::isKeyPressed(Keys::ctrlr))
	{
		if (InputManager::isKeyPressed(Keys::up))
		{
			move[1] += 0.2f;
			moved = true;
		}
		if (InputManager::isKeyPressed(Keys::down))
		{
			move[1] += -0.2f;
			moved = true;
		}
	}
	else
	{
		if (InputManager::isKeyPressed(Keys::up))
		{
			move[2] += -0.2f;
			moved = true;
		}
		if (InputManager::isKeyPressed(Keys::down))
		{
			move[2] += 0.2f;
			moved = true;
		}
	}
	if (InputManager::isKeyPressed(Keys::left))
	{
		move[0] += -0.2f;
		moved = true;
	}
	if (InputManager::isKeyPressed(Keys::right))
	{
		move[0] += 0.2f;
		moved = true;
	}
	if (InputManager::isKeyPressed(Keys::shiftr))
	{
		move *= 3.0f;
	}
	if (m_scroll != 0.0f)
	{
		move[2] -= m_scroll * 0.5f;
		moved = true;
	}
	if (moved)
	{
		move = getRotation(m_gameObject->transformation, 2) * move;
		m_gameObject->translate((glm::vec3)move);
	}

	if (m_alpha < 1.0f)
	{
		rotate();
	}

	m_scroll *= 0.85f;
	if (m_scroll * m_scroll < 0.0005f)
	{
		m_scroll = 0.0f;
	}
}
void CameraControl::setRotation(glm::vec3 dir, bool moveToCenter)
{
	m_alpha = 0.0f;
	m_dir = dir;
	m_dirbkp = (glm::vec3)(m_gameObject->transformation[3] - m_gameObject->transformation[2]);
	m_posbkp = (glm::vec3)m_gameObject->transformation[3];
	m_moveToCenter = moveToCenter;
}
void CameraControl::setScroll(float val) { m_scroll = val; }
void CameraControl::rotate()
{
	if (m_alpha > 0.995f)
	{
		m_alpha = 1.0f;
	}

	glm::vec3 dir = getFullTransform(m_gameObject->parent) * glm::vec4(m_dir, 0.0f);
	glm::vec3 mixe, mixc;

	if (m_moveToCenter)
	{
		mixc = (1 - m_alpha) * m_dirbkp;
		mixe = m_alpha * glm::normalize(-dir) * glm::length(m_posbkp) + (1 - m_alpha) * m_posbkp;
	}
	else
	{
		mixc = m_alpha * (m_posbkp + dir) + (1 - m_alpha) * m_dirbkp;
		mixe = m_posbkp;
	}
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 checkup = glm::normalize(mixc - mixe);

	// if(glm::abs(1-glm::abs( glm::dot(checkup,up)))<0.01f){up=glm::vec3(0.0f
	// ,0.0f, 1.0f); }
	if (glm::dot(m_dir, up) > 0.995f && glm::dot(m_dir, up) < 1.005f)
	{
		up = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	m_gameObject->transformation = glm::inverse(glm::lookAt(mixe, mixc, up));

	m_alpha += 0.04f - m_alpha * 0.021f;
	// m_alpha=1-(1-m_alpha)*0.92f;

	// printf("alpha %f\n", m_alpha);
}
