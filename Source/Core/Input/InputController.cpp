/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "InputController.h"

#include "InputBindings.h"
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
