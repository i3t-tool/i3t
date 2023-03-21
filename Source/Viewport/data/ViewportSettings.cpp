#include "ViewportSettings.h"

namespace Vp
{

// TODO: (DR) Add main scene lighting settings

ViewportSettings::ViewportSettings()
{
	mainScene_cameraMode = AggregateCamera::CameraMode::ORBIT;

	mainScene_orbitCameraRotationX = -90.0f;
	mainScene_orbitCameraRotationY = 30.0f;

	mainScene_iorbitCameraRadius = 12.0f;
	mainScene_iorbitCameraPivot = glm::vec3(0.0f, 0.0f, 0.0f);

	preview_radiusFactor = 1.72f;
	preview_rotateSpeed = 40.0f;

	camera_smoothScroll = true;
	camera_fov = 90.0f;
	camera_zNear = 0.2f;
	camera_zFar = 270.0f;

	orbitCamera_zoomSpeed = 0.8f;
	orbitCamera_rotateSpeed = 0.29f;
	orbitCamera_translateSpeed = 0.02f;

	trackballCamera_zoomSpeed = 0.8f;
	trackballCamera_rotateSpeed = 0.29f;
	trackballCamera_translateSpeed = 0.02f;

	highlight_downscaleFactor = 0.5f;
	highlight_kernelSize = 2;
	highlight_outlineCutoff = 0.2f;
	highlight_useDepth = true;
	highlight_useDepth_darkenFactor = 0.5f;
	highlight_useDepth_desaturateFactor = 0.4f;

	highlight_selectionColor = glm::vec3(0.949, 0.682, 0.18);
	highlight_highlightColor = glm::vec3(0.18, 0.784, 0.949);

	manipulator_size = 0.12f;
}
} // namespace Vp