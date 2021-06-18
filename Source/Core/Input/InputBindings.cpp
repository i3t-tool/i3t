#include "InputBindings.h"

#include "InputManager.h"

std::vector<InputBindings::ActionMapping> g_defaultAction;
std::vector<InputBindings::AxisMapping> g_defaultAxis;

InputBindings::ActionsMap InputBindings::m_inputActions;
InputBindings::AxesMap InputBindings::m_inputAxis;

void InputBindings::init()
{
	// Default input bindings.
	InputManager::setInputAction("copy", Keys::c, { Keys::ctrll });
	InputManager::setInputAction("paste", Keys::v, { Keys::ctrll });
	InputManager::setInputAction("cut", Keys::x, { Keys::ctrll });
	InputManager::setInputAction("duplicate", Keys::d, { Keys::ctrll });
	InputManager::setInputAction("duplicate", Keys::mouseLeft, { Keys::ctrll });
	InputManager::setInputAction("delete", Keys::del);

	InputManager::setInputAxis("scroll", 1.0f,  Keys::mouseScrlUp);
	InputManager::setInputAxis("scroll", -1.0f, Keys::mouseScrlDown);
	InputManager::setInputAxis("pan", 1.0f, Keys::mouseMiddle);
	InputManager::setInputAxis("rotate", 1.0f, Keys::mouseRight);
	InputManager::setInputAxis("rotateAround", 1.0f, Keys::mouseRight, { Keys::altl });

	InputManager::setInputAxis("moveForward", 1.0f, Keys::w);
	InputManager::setInputAxis("moveForward", -1.0f, Keys::s);
	InputManager::setInputAxis("moveRight", 1.0f, Keys::d);
	InputManager::setInputAxis("moveRight", -1.0f, Keys::a);

	InputManager::setInputAction("KeyWorld_mousePan", Keys::mouseMiddle);
	InputManager::setInputAction("KeyWorld_mouseRotate", Keys::mouseRight);
	InputManager::setInputAction("Key_undo", Keys::b);
	InputManager::setInputAction("Key_redo", Keys::n);

	InputManager::setInputAction("test", Keys::t);
	InputManager::setInputAction("MyTestAction", Keys::t, {Keys::ctrll, Keys::altl});
	InputManager::setInputAxis("MyTestAxis", 1.0f, Keys::p, {Keys::ctrll});
	setAxisKey("MyTestAxis", -1.0f, Keys::mouseRight, {Keys::ctrll});

	/*
	InputManager::setInputAxis("MouseScroll", 1.0f, Keys::mouseScrlUp);
	setAxisKey("MouseScroll", -1.0f, Keys::mouseScrlDown);
	 */
}

const std::vector<InputBindings::ActionMapping>& InputBindings::getActionMapping(const char* name)
{
	if (isActionCreated(name))
	{
		return m_inputActions[name];
	}
	return g_defaultAction;
}

bool InputBindings::isActionCreated(const char* name)
{
	return m_inputActions.contains(name);
}

void InputBindings::setAction(const char* name)
{
	m_inputActions[name];
}

void InputBindings::setActionKey(const char* name, Keys::Code code, ModifiersList modifiers)
{
	/// \todo MH Check for conflicts.
	if (isActionCreated(name))
	{
		m_inputActions[name].push_back({code, createModifiers(modifiers)});
	}
}

void InputBindings::removeActionKey(const char* name, Keys::Code code)
{
	if (isActionCreated(name))
	{
		std::erase_if(m_inputActions[name], [&code](ActionMapping action) { return code == action.code; });
	}
}

std::vector<InputBindings::AxisMapping> InputBindings::getAxisMapping(const char* name)
{
	if (isAxisCreated(name))
	{
		return m_inputAxis[name];
	}
	return g_defaultAxis;
}

bool InputBindings::isAxisCreated(const char* name)
{
	return m_inputAxis.contains(name);
}

void InputBindings::setAxis(const char* name)
{
	m_inputAxis[name];
}

void InputBindings::setAxisKey(const char* name, float scale, Keys::Code code, ModifiersList modifiers)
{
	if (InputBindings::isAxisCreated(name))
	{
		m_inputAxis[name].push_back({code, scale, createModifiers(modifiers)});
	}
}

void InputBindings::removeAxisKey(const char* name, Keys::Code code)
{
	if (isAxisCreated(name))
	{
		std::erase_if(m_inputAxis[name], [&code](AxisMapping axis) { return axis.code == code; });
	}
}
