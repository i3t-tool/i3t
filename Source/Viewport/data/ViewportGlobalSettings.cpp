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
#include "ViewportGlobalSettings.h"

#include "rttr/registration.h"

#include "Viewport/shader/PhongShader.h"

namespace Vp
{

RTTR_REGISTRATION
{
	rttr::registration::class_<GlobalCameraSettings>("GlobalCameraSettings")
	    .property("smoothScroll", &GlobalCameraSettings::smoothScroll)
	    .property("smoothScrollDamping", &GlobalCameraSettings::smoothScrollDamping)
	    .property("orbitCamera_zoomSpeed", &GlobalCameraSettings::orbit_zoomSpeed)
	    .property("orbitCamera_rotateSpeed", &GlobalCameraSettings::orbit_rotateSpeed)
	    .property("orbitCamera_translateSpeed", &GlobalCameraSettings::orbit_translateSpeed)
	    .property("trackballCamera_zoomSpeed", &GlobalCameraSettings::trackball_zoomSpeed)
	    .property("trackballCamera_rotateSpeed", &GlobalCameraSettings::trackball_rotateSpeed)
	    .property("trackballCamera_translateSpeed", &GlobalCameraSettings::trackball_translateSpeed);

	rttr::registration::class_<HighlightSettings>("HighlightSettings")
	    .property("downscaleFactor", &HighlightSettings::downscaleFactor)
	    .property("kernelSize", &HighlightSettings::kernelSize)
	    .property("outlineCutoff", &HighlightSettings::outlineCutoff)
	    .property("useDepth", &HighlightSettings::useDepth)
	    .property("useDepth_darkenFactor", &HighlightSettings::useDepth_darkenFactor)
	    .property("useDepth_desaturateFactor", &HighlightSettings::useDepth_desaturateFactor)
	    .property("selectionColor", &HighlightSettings::selectionColor)
	    .property("highlightColor", &HighlightSettings::highlightColor);

	rttr::registration::class_<GridSettings>("GridSettings")
	    .property("color", &GridSettings::color)
	    .property("axisXColor", &GridSettings::axisXColor)
	    .property("axisYColor", &GridSettings::axisYColor)
	    .property("axisZColor", &GridSettings::axisZColor)
	    .property("size", &GridSettings::size)
	    .property("strength", &GridSettings::strength)
	    .property("lineWidth", &GridSettings::lineWidth)
	    .property("grid1FadeStart", &GridSettings::grid1FadeStart)
	    .property("grid1FadeEnd", &GridSettings::grid1FadeEnd)
	    .property("grid2FadeStart", &GridSettings::grid2FadeStart)
	    .property("grid2FadeEnd", &GridSettings::grid2FadeEnd);

	rttr::registration::class_<ViewportGlobalSettings>("ViewportGlobalSettings")
	    .property("preview_fov", &ViewportGlobalSettings::preview_fov)
	    .property("preview_radiusFactor", &ViewportGlobalSettings::preview_radiusFactor)
	    .property("preview_rotateSpeed", &ViewportGlobalSettings::preview_rotateSpeed)
	    .property("lighting_lightingModel", &ViewportGlobalSettings::lighting_lightingModel)
	    .property("manipulator_size", &ViewportGlobalSettings::manipulator_size)
	    .property("camera", &ViewportGlobalSettings::camera)
	    .property("highlight", &ViewportGlobalSettings::highlight)
	    .property("grid", &ViewportGlobalSettings::grid)
	    .property("localGrid", &ViewportGlobalSettings::localGrid);

	rttr::registration::enumeration<PhongShader::LightingModel>("LightingModel")(
	    rttr::value("ORBIT", PhongShader::LightingModel::PHONG),
	    rttr::value("TRACKBALL", PhongShader::LightingModel::BLINN_PHONG));
}

// TODO: (DR) Add main scene lighting settings

GlobalCameraSettings::GlobalCameraSettings()
{
	smoothScroll = true;
	smoothScrollDamping = 0.85f;

	orbit_zoomSpeed = 0.8f;
	orbit_rotateSpeed = 0.29f;
	orbit_translateSpeed = 0.02f;

	trackball_zoomSpeed = 0.8f;
	trackball_rotateSpeed = 0.29f;
	trackball_translateSpeed = 0.02f;
}

HighlightSettings::HighlightSettings()
{
	downscaleFactor = 0.7f;
	kernelSize = 2;
	outlineCutoff = 0.18f;
	useDepth = true;
	useDepth_darkenFactor = 0.5f;
	useDepth_desaturateFactor = 0.4f;

	selectionColor = glm::vec3(0.949, 0.682, 0.18);
	highlightColor = glm::vec3(0.18, 0.784, 0.949);
}

GridSettings::GridSettings() : GridSettings(false) {}
GridSettings::GridSettings(bool localGrid)
{
	if (!localGrid)
	{
		color = glm::vec3(0.45, 0.49, 0.53);
		axisXColor = glm::vec3(1.0, 0.49, 0.53);
		axisYColor = glm::vec3(0.41, 0.96, 0.49);
		axisZColor = glm::vec3(0.45, 0.49, 1.0);

		size = 1.0f;
		strength = 0.51f;
		lineWidth = 1.08f;

		grid1FadeStart = 0.0f;
		grid1FadeEnd = 0.23f;
		grid2FadeStart = 0.0f;
		grid2FadeEnd = 0.9f;
	}
	else
	{
		color = glm::vec3(0.412, 0.40, 0.211);
		axisXColor = glm::vec3(0.87, 0.01, 1.0);
		axisYColor = glm::vec3(0.60, 1.0, 0.02);
		axisZColor = glm::vec3(0.00, 0.9, 1.0);

		size = 1.0f;
		strength = 0.5f;
		lineWidth = 1.0f;

		grid1FadeStart = 0.0f;
		grid1FadeEnd = 0.12f;
		grid2FadeStart = 0.0f;
		grid2FadeEnd = 1.0f;
	}
}

ViewportGlobalSettings::ViewportGlobalSettings()
{
	preview_fov = 30.0f;
	preview_radiusFactor = 1.38f;
	preview_rotateSpeed = 28.0f;

	lighting_lightingModel = PhongShader::LightingModel::BLINN_PHONG;

	manipulator_size = 0.14f;
}

} // namespace Vp