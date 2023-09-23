#pragma once

#include "glm/glm.hpp"

#include "Viewport/camera/AggregateCamera.h"

namespace Vp
{
struct ViewportSceneSettings
{
	ViewportSceneSettings();

	AggregateCamera::CameraMode mainScene_cameraMode;

	float mainScene_orbitCameraRotationX;
	float mainScene_orbitCameraRotationY;

	glm::mat4 mainScene_trackballCameraRotation;

	float mainScene_iorbitCameraRadius;
	glm::vec3 mainScene_iorbitCameraPivot;

	bool mainScene_lightFollowsCamera;

	float camera_fov;
	float camera_zNear;
	float camera_zFar;

	bool manipulator_enabled;
};

} // namespace Vp
