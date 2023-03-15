#pragma once

#include "glm/glm.hpp"

#include "Viewport/camera/AggregateCamera.h"

namespace Vp
{
/**
 * General viewport settings, some used only for initialisation, some used during runtime.
 */
class ViewportSettings
{
public:
	AggregateCamera::CameraMode mainScene_cameraMode;

	float mainScene_orbitCameraRotationX;
	float mainScene_orbitCameraRotationY;
	float mainScene_iorbitCameraRadius;
	glm::vec3 mainScene_iorbitCameraPivot;

	float preview_radiusFactor;
	float preview_rotateSpeed;

	bool camera_smoothScroll;
	float camera_fov;
	float camera_zNear;
	float camera_zFar;

	float orbitCamera_zoomSpeed;
	float orbitCamera_rotateSpeed;
	float orbitCamera_translateSpeed;

	float trackballCamera_zoomSpeed;
	float trackballCamera_rotateSpeed;
	float trackballCamera_translateSpeed;

	float highlight_downscaleFactor;
	int highlight_kernelSize;
	float highlight_outlineCutoff;
	bool highlight_useDepth;
	float highlight_useDepth_darkenFactor;
	float highlight_useDepth_desaturateFactor;

	ViewportSettings();
};

} // namespace Vp
