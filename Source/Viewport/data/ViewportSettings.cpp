#include "ViewportSettings.h"
#include "Viewport/shader/PhongShader.h"

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

	mainScene_lightFollowsCamera = true;

	preview_fov = 30.0f;
	preview_radiusFactor = 1.38f;
	preview_rotateSpeed = 28.0f;

	camera_smoothScroll = false;
	camera_fov = 90.0f;
	camera_zNear = 0.2f;
	camera_zFar = 270.0f;

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

	manipulator_enabled = true;
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