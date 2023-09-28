#pragma once

#include "glm/glm.hpp"

#include "Viewport/camera/AggregateCamera.h"

namespace Vp
{

struct CameraSettings
{
	CameraSettings();

	AggregateCamera::CameraMode mode;

	float orbitCameraRotationX;
	float orbitCameraRotationY;

	glm::mat4 trackballCameraRotation;

	float iorbitCameraRadius;
	glm::vec3 iorbitCameraPivot;

	float fov;
	float zNear;
	float zFar;
};

struct MainSceneSettings
{
	MainSceneSettings();

	bool lightFollowsCamera;

	CameraSettings camera;
};

struct ViewportSceneSettings
{
	ViewportSceneSettings();

	bool manipulator_enabled;

	MainSceneSettings mainScene;
};


} // namespace Vp
