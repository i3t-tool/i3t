#include "InputController.h"

#include "InputManager.h"

InputController::InputController()
{
	InputManager::addInputController(this);
}

void InputController::addKeyDownFn(Keys::Code key, KeyCallback fn)
{
	m_keyDownCallbacks.insert(std::pair(key, fn));
}

void InputController::addKeyFn(Keys::Code key, KeyCallback fn)
{
	m_keyCallbacks.insert(std::pair(key, fn));
}
