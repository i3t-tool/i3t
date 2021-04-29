#pragma once

#include <map>
#include <unordered_map>

#include "KeyCodes.h"

using MActions = std::map<std::string_view, Keys::Code>;

struct InputActions final
{
	using MAxis = std::unordered_map<std::string_view, std::pair<Keys::Code, float>>;

	friend class InputManager;

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

	bool isActionCreated(const char* name);
	bool isAxisCreated(const char* name);

private:
  static MActions m_inputActions;
	static MAxis m_inputAxis;
};
