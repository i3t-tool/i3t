#pragma once

#include "Viewport/data/ViewportSceneSettings.h"

struct ViewportWindowSettings
{
	Vp::CameraSettings camera;
	bool manipulator_enabled{true};
	bool showLocalGrid{true};
	bool clipFrustum{true}; ///< Whether to clip the outside of camera frustum when camera tracking
};
