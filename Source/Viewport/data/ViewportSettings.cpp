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
}
} // namespace Vp