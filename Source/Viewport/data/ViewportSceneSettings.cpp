/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "ViewportSceneSettings.h"

#include "rttr/registration.h"

namespace Vp
{
RTTR_REGISTRATION
{
	rttr::registration::class_<CameraSettings>("CameraSettings")
	    .property("mode", &CameraSettings::mode)
	    .property("orbitCameraRotationX", &CameraSettings::orbitCameraRotationX)
	    .property("orbitCameraRotationY", &CameraSettings::orbitCameraRotationY)
	    .property("trackballCameraRotation", &CameraSettings::trackballCameraRotation)
	    .property("iorbitCameraRadius", &CameraSettings::iorbitCameraRadius)
	    .property("iorbitCameraPivot", &CameraSettings::iorbitCameraPivot)
	    .property("fov", &CameraSettings::fov)
	    .property("zNear", &CameraSettings::zNear)
	    .property("zFar", &CameraSettings::zFar);

	rttr::registration::class_<MainSceneSettings>("MainSceneSettings")
	    .property("lightFollowsCamera", &MainSceneSettings::lightFollowsCamera)
	    .property("camera", &MainSceneSettings::camera);

	rttr::registration::class_<ViewportSceneSettings>("ViewportSceneSettings")
	    .property("manipulator_enabled", &ViewportSceneSettings::manipulator_enabled)
	    .property("mainScene", &ViewportSceneSettings::mainScene);

	rttr::registration::enumeration<AggregateCamera::CameraMode>("CameraMode")(
	    rttr::value("ORBIT", AggregateCamera::CameraMode::ORBIT),
	    rttr::value("TRACKBALL", AggregateCamera::CameraMode::TRACKBALL),
	    rttr::value("NONE", AggregateCamera::CameraMode::NONE));
}

CameraSettings::CameraSettings()
{
	mode = AggregateCamera::CameraMode::ORBIT;

	orbitCameraRotationX = -90.0f;
	orbitCameraRotationY = 30.0f;

	trackballCameraRotation = glm::mat4(1.0f);

	iorbitCameraRadius = 12.0f;
	iorbitCameraPivot = glm::vec3(0.0f, 0.0f, 0.0f);

	fov = 90.0f;
	zNear = 0.2f;
	zFar = 270.0f;
}

MainSceneSettings::MainSceneSettings()
{
	lightFollowsCamera = true;
}

ViewportSceneSettings::ViewportSceneSettings()
{
	manipulator_enabled = true;
}

} // namespace Vp