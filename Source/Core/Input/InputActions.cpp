#include "InputActions.h"

#include "Rendering/GUIProjection.h"

/// \todo Set default keys according to the Dr. Felkel's table.
bool InputActions::CameraOrbit = true;
Keys::Code InputActions::KeyWorld_mousePan = Keys::mouseMiddle;
Keys::Code InputActions::KeyWorld_mouseRotate = Keys::mouseRight;

Keys::Code InputActions::KeyScene_camTo_orbitX = Keys::n1;
Keys::Code InputActions::KeyScene_camTo_orbitY = Keys::n2;
Keys::Code InputActions::KeyScene_camTo_orbitZ = Keys::n3;
Keys::Code InputActions::KeyScene_camTo_worldX = Keys::n4;
Keys::Code InputActions::KeyScene_camTo_worldY = Keys::n5;
Keys::Code InputActions::KeyScene_camTo_worldZ = Keys::n6;
Keys::Code InputActions::KeyScene_camTo_scene = Keys::n0;

Keys::Code InputActions::Key_undo = Keys::b;
Keys::Code InputActions::Key_redo = Keys::n;

void InputActions::resize(float width, float height)
{
	InputManager::setScreenSize((int)width, (int)height);
	GUIProjection::setScreenSize(width, height);
}
