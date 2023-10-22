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

namespace Vp
{

struct GlobalCameraSettings
{
public:
	GlobalCameraSettings();

	bool smoothScroll;

	float orbit_zoomSpeed;
	float orbit_rotateSpeed;
	float orbit_translateSpeed;

	float trackball_zoomSpeed;
	float trackball_rotateSpeed;
	float trackball_translateSpeed;
};

struct HighlightSettings
{
public:
	HighlightSettings();

	float downscaleFactor;
	int kernelSize;
	float outlineCutoff;
	bool useDepth;
	float useDepth_darkenFactor;
	float useDepth_desaturateFactor;

	glm::vec3 selectionColor;
	glm::vec3 highlightColor;
};

struct GridSettings
{
public:
	GridSettings();

	glm::vec3 color;
	glm::vec3 axisXColor;
	glm::vec3 axisYColor;
	glm::vec3 axisZColor;

	float size;
	float strength;
	float lineWidth;

	float grid1FadeStart;
	float grid1FadeEnd;
	float grid2FadeStart;
	float grid2FadeEnd;
};

/**
 * Global viewport settings, some used only for initialisation, some used during runtime.
 */
struct ViewportGlobalSettings
{
public:
	ViewportGlobalSettings();

	float preview_fov;
	float preview_radiusFactor;
	float preview_rotateSpeed;

	int lighting_lightingModel;

	//	bool manipulator_enabled;
	float manipulator_size;

	GlobalCameraSettings camera;
	HighlightSettings highlight;
	GridSettings grid;
};

} // namespace Vp
