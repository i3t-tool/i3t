#include "Settings.h"

#include "Rendering/GUIProjection.h"

/// \todo Set default keys according to the Dr. Felkel's table.
bool Settings::CameraOrbit = true;
Keys::Code Settings::KeyWorld_mousePan = Keys::mouseMiddle;
Keys::Code Settings::KeyWorld_mouseRotate = Keys::mouseRight;

Keys::Code Settings::KeyScene_camTo_orbitX = Keys::n1;
Keys::Code Settings::KeyScene_camTo_orbitY = Keys::n2;
Keys::Code Settings::KeyScene_camTo_orbitZ = Keys::n3;
Keys::Code Settings::KeyScene_camTo_worldX = Keys::n4;
Keys::Code Settings::KeyScene_camTo_worldY = Keys::n5;
Keys::Code Settings::KeyScene_camTo_worldZ = Keys::n6;
Keys::Code Settings::KeyScene_camTo_scene = Keys::n0;

Keys::Code Settings::Key_undo = Keys::b;
Keys::Code Settings::Key_redo = Keys::n;

void Settings::resize(float width, float height)
{
	InputController::setScreenSize((int)width, (int)height);
	GUIProjection::setScreenSize(width, height);
}
