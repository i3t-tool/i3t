#include "InputBindings.h"

#include "InputManager.h"

std::vector<Keys::Code> g_defaultAction;
std::vector<std::pair<Keys::Code, float>> g_defaultAxis;

InputBindings::ActionsMap InputBindings::m_inputActions;
InputBindings::AxisMap InputBindings::m_inputAxis;

void InputBindings::init()
{
	// Default input bindings.
	InputManager::setInputAction("KeyWorld_mousePan", Keys::mouseMiddle);
	InputManager::setInputAction("KeyWorld_mouseRotate", Keys::mouseRight);
	InputManager::setInputAction("Key_undo", Keys::b);
	InputManager::setInputAction("Key_redo", Keys::n);
}

const std::vector<Keys::Code>& InputBindings::getActionKeys(const char* name)
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

void InputBindings::setActionKey(const char* name, Keys::Code code)
{
	/// \todo MH Check for conflicts.
  if (isActionCreated(name))
  {
		auto it = std::find(m_inputActions[name].begin(), m_inputActions[name].end(), code);
		if (it == m_inputActions[name].end())
    {
			m_inputActions[name].push_back(code);
		}
	}
}

void InputBindings::removeActionKey(const char* name, Keys::Code code)
{
	if (isActionCreated(name))
  {
		std::erase_if(m_inputActions[name],
		              [&code](auto c) { return code == c; });
	}
}

std::vector<std::pair<Keys::Code, float>> InputBindings::getAxisMappings(const char* name)
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

void InputBindings::removeAxisKey(const char* name, Keys::Code code)
{
	if (isAxisCreated(name))
  {
		std::erase_if(m_inputAxis[name],
		              [&code](std::pair<Keys::Code, float> pair) { return pair.first == code; });
	}
}
