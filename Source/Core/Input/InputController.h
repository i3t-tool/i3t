#pragma once

#include <functional>
#include <map>

#include "InputBindings.h"
#include "KeyCodes.h"

using KeyCallback = std::function<void()>;
using AxisCallback = std::function<void(float)>;

class InputController
{
	friend class InputManager;

	struct Action
  {
    std::string_view name;
    EKeyState state;
		KeyCallback fn;
	};

	std::map<Keys::Code, KeyCallback> m_keyCallbacks;
	std::map<Keys::Code, KeyCallback> m_keyDownCallbacks;

  std::vector<Action> m_actions;
	std::unordered_map<std::string_view, AxisCallback> m_axis;

public:
	InputController();
	void addKeyDownFn(Keys::Code key, KeyCallback fn);
	void addKeyFn(Keys::Code key, KeyCallback fn);

	void bindAction(const char* name, EKeyState state, KeyCallback fn);
	void bindAxis(const char* name, AxisCallback fn);
};
