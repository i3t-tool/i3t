#include "CameraControl.h"
#include "../Component.h"
#include "../Transforms.h"
#include "Core/Input/InputBindings.h"
#include "Core/Input/InputManager.h"
#include "GUI/Elements/Windows/ViewportWindow.h"
#include <iostream>
#include <typeinfo>

const char* CameraControl::s_type = NULL;

CameraControl::CameraControl()
{
	CameraControl::s_type = typeid(CameraControl).name();
	this->m_type = CameraControl::s_type;
}

void CameraControl::update()
{
	if (!InputManager::isFocused<UI::Viewport>())
		return;

	float x = InputManager::m_mouseXDelta / 8.0f;
	float y = InputManager::m_mouseYDelta / 8.0f;

	if (InputManager::isKeyPressed(Keys::mouseMiddle))
	{
		glm::vec4 move =
				(getRotation(m_gameObject->transformation, 2) * glm::vec4(x, 0.0f, 0.0f, 0.0f) + glm::vec4(0.0f, y, 0.0f, 0.0f)) *
				0.25f;
		if (InputManager::isKeyPressed(Keys::shiftr))
		{
			move *= 3.0f;
		}
		m_gameObject->translate(glm::vec3(-move.x, move.y, -move.z));
	}
	else if (InputManager::isKeyPressed(Keys::mouseRight))
	{
		m_gameObject->rotateAround((glm::vec3)m_gameObject->transformation[0], -y, (glm::vec3)m_gameObject->transformation[3]);
		m_gameObject->rotateAround(glm::vec3(0.0f, 1.0f, 0.0f), -x, (glm::vec3)m_gameObject->transformation[3]);
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
	if(World::scroll!=0.0f){
		move[2] -= World::scroll * 0.5f;
		moved = true;
	}
	if (moved)
	{
		move = getRotation(m_gameObject->transformation, 2) * move;
		m_gameObject->translate((glm::vec3)move);
	}
}

void CameraControl::setRotation(float x, float y)
{
	/*this->rotyNeg=-y;
	this->rotxNeg=-x;

	glm::mat4 roty = glm::rotate(glm::mat4(1.0f), glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotx = glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));

	m_gameObject->rot=rotx*roty;
	m_gameObject->translate(glm::vec3(0.0f));	*/
}
