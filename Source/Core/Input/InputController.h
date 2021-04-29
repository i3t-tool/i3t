#pragma once

#include <functional>

#include "InputBindings.h"
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
	};

  std::vector<Action> m_actions;
	std::unordered_map<std::string, AxisCallback> m_axis;

public:
	InputController();

	void bindAction(const char* name, EKeyState state, KeyCallback fn);
	void bindAxis(const char* name, AxisCallback fn);
};
