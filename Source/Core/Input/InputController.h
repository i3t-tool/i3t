
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
