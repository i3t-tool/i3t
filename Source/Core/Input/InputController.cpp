#include "InputController.h"

#include "InputManager.h"

InputController::InputController()
{
	InputManager::addInputController(this);
}

void InputController::bindAction(const char* name, EKeyState state, KeyCallback fn)
{
	if (InputBindings::isActionCreated(name))
		m_actions.emplace_back(Action{name, state, fn});
}

void InputController::bindAxis(const char* name, AxisCallback fn)
{
	if (InputBindings::isAxisCreated(name))
		m_axis.insert({name, fn});
}
