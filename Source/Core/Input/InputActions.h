#pragma once

#include "Core/Input/InputManager.h"

struct InputActions final
{
	static bool CameraOrbit;
	static Keys::Code KeyWorld_mousePan;
	static Keys::Code KeyWorld_mouseRotate;
	static Keys::Code KeyScene_camTo_orbitX;
	static Keys::Code KeyScene_camTo_orbitY;
	static Keys::Code KeyScene_camTo_orbitZ;
	static Keys::Code KeyScene_camTo_worldX;
	static Keys::Code KeyScene_camTo_worldY;
	static Keys::Code KeyScene_camTo_worldZ;
	static Keys::Code KeyScene_camTo_scene;

	static Keys::Code Key_undo;
	static Keys::Code Key_redo;

	static void resize(float width, float height);
};
