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

#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>

#include "KeyCodes.h"

using KeyCallback = std::function<void()>;
using AxisCallback = std::function<void(float)>;

class InputController
{
	friend class InputManager;

	struct Action
	{
		std::string name;
		EKeyState state;
		KeyCallback fn;

		Action(std::string n, EKeyState s, KeyCallback callback) : name(std::move(n)), state(s), fn(std::move(callback))
		{}
	};

	std::vector<Action> m_actions;
	std::unordered_map<std::string, AxisCallback> m_axis;

public:
	InputController();

	/**
	 * See InputBindings.cpp file for default actions.
	 */
	void bindAction(const char* name, EKeyState state, KeyCallback fn);
	void bindAxis(const char* name, AxisCallback fn);

	/**
	 * Make action active.
	 */
	void triggerAction(const char* name, EKeyState state);
};
