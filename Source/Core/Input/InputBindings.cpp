#include "InputBindings.h"

#include "InputManager.h"

/// \todo Set default keys according to the Dr. Felkel's table.
bool InputBindings::CameraOrbit = true;
Keys::Code InputBindings::KeyWorld_mousePan = Keys::mouseMiddle;
Keys::Code InputBindings::KeyWorld_mouseRotate = Keys::mouseRight;

Keys::Code InputBindings::KeyScene_camTo_orbitX = Keys::n1;
Keys::Code InputBindings::KeyScene_camTo_orbitY = Keys::n2;
Keys::Code InputBindings::KeyScene_camTo_orbitZ = Keys::n3;
Keys::Code InputBindings::KeyScene_camTo_worldX = Keys::n4;
Keys::Code InputBindings::KeyScene_camTo_worldY = Keys::n5;
Keys::Code InputBindings::KeyScene_camTo_worldZ = Keys::n6;
Keys::Code InputBindings::KeyScene_camTo_scene = Keys::n0;

Keys::Code InputBindings::Key_undo = Keys::b;
Keys::Code InputBindings::Key_redo = Keys::n;

MActions InputBindings::m_inputActions;
InputBindings::MAxis InputBindings::m_inputAxis;

void InputBindings::resize(float width, float height)
{
	InputManager::setScreenSize((int)width, (int)height);
	// GUIProjection::setScreenSize(width, height);
}

bool InputBindings::isActionCreated(const char* name)
{
	return m_inputActions.contains(name);
}

bool InputBindings::isAxisCreated(const char* name)
{
  return m_inputAxis.contains(name);
}

Keys::Code InputBindings::getActionKey(const char* name)
{
	if (isAxisCreated(name))
  {
		return m_inputActions[name];
	}
	return Keys::a;
}

void InputBindings::setActionKey(const char* name, Keys::Code code)
{
	/// \todo MH Check for conflicts.
  if (isActionCreated(name))
  {
		auto& ac = m_inputActions;
		m_inputActions[name] = code;
		auto& a = m_inputActions;
	}
}
