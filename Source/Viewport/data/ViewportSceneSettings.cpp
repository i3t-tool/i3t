#include "ViewportSceneSettings.h"

#include "rttr/registration.h"

namespace Vp
{
RTTR_REGISTRATION
{
	rttr::registration::class_<ViewportSceneSettings>("ViewportSceneSettings")
	    .property("mainScene_cameraMode", &ViewportSceneSettings::mainScene_cameraMode)
	    .property("mainScene_orbitCameraRotationX", &ViewportSceneSettings::mainScene_orbitCameraRotationX)
	    .property("mainScene_orbitCameraRotationY", &ViewportSceneSettings::mainScene_orbitCameraRotationY)
	    .property("mainScene_trackballCameraRotation", &ViewportSceneSettings::mainScene_trackballCameraRotation)
	    .property("mainScene_iorbitCameraRadius", &ViewportSceneSettings::mainScene_iorbitCameraRadius)
	    .property("mainScene_iorbitCameraPivot", &ViewportSceneSettings::mainScene_iorbitCameraPivot)
	    .property("mainScene_lightFollowsCamera", &ViewportSceneSettings::mainScene_lightFollowsCamera)
	    .property("camera_fov", &ViewportSceneSettings::camera_fov)
	    .property("camera_zNear", &ViewportSceneSettings::camera_zNear)
	    .property("camera_zFar", &ViewportSceneSettings::camera_zFar)
	    .property("manipulator_enabled", &ViewportSceneSettings::manipulator_enabled);

	rttr::registration::enumeration<AggregateCamera::CameraMode>("CameraMode")(
	    rttr::value("ORBIT", AggregateCamera::CameraMode::ORBIT),
	    rttr::value("TRACKBALL", AggregateCamera::CameraMode::TRACKBALL),
	    rttr::value("NONE", AggregateCamera::CameraMode::NONE));
}

ViewportSceneSettings::ViewportSceneSettings()
{
	mainScene_cameraMode = AggregateCamera::CameraMode::ORBIT;

	mainScene_orbitCameraRotationX = -90.0f;
	mainScene_orbitCameraRotationY = 30.0f;

	mainScene_trackballCameraRotation = glm::mat4(1.0f);

	mainScene_iorbitCameraRadius = 12.0f;
	mainScene_iorbitCameraPivot = glm::vec3(0.0f, 0.0f, 0.0f);

	mainScene_lightFollowsCamera = true;

	camera_fov = 90.0f;
	camera_zNear = 0.2f;
	camera_zFar = 270.0f;

	manipulator_enabled = true;
}

} // namespace Vp