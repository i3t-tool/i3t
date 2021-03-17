#pragma once

#include <functional>
#include <map>

#include "KeyCodes.h"

using KeyCallback = std::function<void()>;

class InputController
{
	friend class InputManager;

	std::map<Keys::Code, KeyCallback> m_keyCallbacks;
	std::map<Keys::Code, KeyCallback> m_keyDownCallbacks;

public:
	InputController();
	void addKeyDownFn(Keys::Code key, KeyCallback fn);
	void addKeyFn(Keys::Code key, KeyCallback fn);
};
