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


} // namespace Vp
