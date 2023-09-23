#include "ViewportGlobalSettings.h"

#include "rttr/registration.h"

#include "Viewport/shader/PhongShader.h"

namespace Vp
{

RTTR_REGISTRATION
{
	rttr::registration::class_<ViewportGlobalSettings>("ViewportGlobalSettings")
	    .property("preview_fov", &ViewportGlobalSettings::preview_fov)
	    .property("preview_radiusFactor", &ViewportGlobalSettings::preview_radiusFactor)
	    .property("preview_rotateSpeed", &ViewportGlobalSettings::preview_rotateSpeed)
	    .property("camera_smoothScroll", &ViewportGlobalSettings::camera_smoothScroll)
	    .property("orbitCamera_zoomSpeed", &ViewportGlobalSettings::orbitCamera_zoomSpeed)
	    .property("orbitCamera_rotateSpeed", &ViewportGlobalSettings::orbitCamera_rotateSpeed)
	    .property("orbitCamera_translateSpeed", &ViewportGlobalSettings::orbitCamera_translateSpeed)
	    .property("trackballCamera_zoomSpeed", &ViewportGlobalSettings::trackballCamera_zoomSpeed)
	    .property("trackballCamera_rotateSpeed", &ViewportGlobalSettings::trackballCamera_rotateSpeed)
	    .property("trackballCamera_translateSpeed", &ViewportGlobalSettings::trackballCamera_translateSpeed)
	    .property("lighting_lightingModel", &ViewportGlobalSettings::lighting_lightingModel)
	    .property("highlight_downscaleFactor", &ViewportGlobalSettings::highlight_downscaleFactor)
	    .property("highlight_kernelSize", &ViewportGlobalSettings::highlight_kernelSize)
	    .property("highlight_outlineCutoff", &ViewportGlobalSettings::highlight_outlineCutoff)
	    .property("highlight_useDepth", &ViewportGlobalSettings::highlight_useDepth)
	    .property("highlight_useDepth_darkenFactor", &ViewportGlobalSettings::highlight_useDepth_darkenFactor)
	    .property("highlight_useDepth_desaturateFactor", &ViewportGlobalSettings::highlight_useDepth_desaturateFactor)
	    .property("highlight_selectionColor", &ViewportGlobalSettings::highlight_selectionColor)
	    .property("highlight_highlightColor", &ViewportGlobalSettings::highlight_highlightColor)
	    .property("manipulator_size", &ViewportGlobalSettings::manipulator_size)
	    .property("grid_color", &ViewportGlobalSettings::grid_color)
	    .property("grid_axisXColor", &ViewportGlobalSettings::grid_axisXColor)
	    .property("grid_axisYColor", &ViewportGlobalSettings::grid_axisYColor)
	    .property("grid_axisZColor", &ViewportGlobalSettings::grid_axisZColor)
	    .property("grid_size", &ViewportGlobalSettings::grid_size)
	    .property("grid_strength", &ViewportGlobalSettings::grid_strength)
	    .property("grid_lineWidth", &ViewportGlobalSettings::grid_lineWidth)
	    .property("grid_grid1FadeStart", &ViewportGlobalSettings::grid_grid1FadeStart)
	    .property("grid_grid1FadeEnd", &ViewportGlobalSettings::grid_grid1FadeEnd)
	    .property("grid_grid2FadeStart", &ViewportGlobalSettings::grid_grid2FadeStart)
	    .property("grid_grid2FadeEnd", &ViewportGlobalSettings::grid_grid2FadeEnd);

	rttr::registration::enumeration<PhongShader::LightingModel>("LightingModel")(
	    rttr::value("ORBIT", PhongShader::LightingModel::PHONG),
	    rttr::value("TRACKBALL", PhongShader::LightingModel::BLINN_PHONG));
}

// TODO: (DR) Add main scene lighting settings

ViewportGlobalSettings::ViewportGlobalSettings()
{
	preview_fov = 30.0f;
	preview_radiusFactor = 1.38f;
	preview_rotateSpeed = 28.0f;

	camera_smoothScroll = false;

	orbitCamera_zoomSpeed = 0.8f;
	orbitCamera_rotateSpeed = 0.29f;
	orbitCamera_translateSpeed = 0.02f;

	trackballCamera_zoomSpeed = 0.8f;
	trackballCamera_rotateSpeed = 0.29f;
	trackballCamera_translateSpeed = 0.02f;

	lighting_lightingModel = PhongShader::LightingModel::BLINN_PHONG;

	highlight_downscaleFactor = 0.5f;
	highlight_kernelSize = 2;
	highlight_outlineCutoff = 0.2f;
	highlight_useDepth = true;
	highlight_useDepth_darkenFactor = 0.5f;
	highlight_useDepth_desaturateFactor = 0.4f;

	highlight_selectionColor = glm::vec3(0.949, 0.682, 0.18);
	highlight_highlightColor = glm::vec3(0.18, 0.784, 0.949);

	//	manipulator_enabled = true;
	manipulator_size = 0.14f;

	grid_color = glm::vec3(0.45, 0.49, 0.53);
	grid_axisXColor = glm::vec3(1.0, 0.49, 0.53);
	grid_axisYColor = glm::vec3(0.41, 0.96, 0.49);
	grid_axisZColor = glm::vec3(0.45, 0.49, 1.0);

	grid_size = 1.0f;
	grid_strength = 0.5f;
	grid_lineWidth = 1.0f;
	grid_grid1FadeStart = 0.0f;
	grid_grid1FadeEnd = 0.23f;
	grid_grid2FadeStart = 0.0f;
	grid_grid2FadeEnd = 0.9f;
}
} // namespace Vp