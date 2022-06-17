#include "InputController.h"

#include "InputBindings.h"
#include "InputManager.h"

InputController::InputController() { InputManager::addInputController(this); }

void InputController::bindAction(const char* name, EKeyState state, KeyCallback fn)
{
	if (InputBindings::isActionCreated(name)) m_actions.emplace_back(Action{name, state, fn});
}

void InputController::bindAxis(const char* name, AxisCallback fn)
{
	if (InputBindings::isAxisCreated(name)) m_axis.insert({name, fn});
}

void InputController::triggerAction(const char* name, EKeyState state)
{
	if (InputBindings::isActionCreated(name))
	{
		for (const auto& action : m_actions)
		{
			if (action.state == state && strcmp(action.name.c_str(), name) == 0)
			{
				action.fn();
				break;
			}
		}
	}
}
