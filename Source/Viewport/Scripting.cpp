/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Scripting/Environment.h"

#include "Core/Application.h"
#include "Viewport/Viewport.h"
#include "Viewport/data/ViewportSettings.h"

LUA_REGISTRATION
{
	auto api = L["I3T"];
	api["viewport"] = L.create_table();

	auto viewport = api["viewport"];

	/*
	L.new_usertype<Vp::ViewportGlobalSettings>(
	    "ViewportGlobalSettings"
	);
	 */

	// clang-format off
	L.new_usertype<Vp::CameraSettings>(
		"CameraSettings",
	    "orbit_camera_rotation_x", &Vp::CameraSettings::orbitCameraRotationX,
	    "orbit_camera_rotation_y", &Vp::CameraSettings::orbitCameraRotationY,
	    "orbit_camera_radius", &Vp::CameraSettings::iorbitCameraRadius,
	    "orbit_camera_pivot", &Vp::CameraSettings::iorbitCameraPivot,
	    "fov", &Vp::CameraSettings::fov,
	    "near", &Vp::CameraSettings::zNear,
	    "far", &Vp::CameraSettings::zFar
	);
	// clang-format on

	viewport["get_settings"] = []() -> Vp::CameraSettings& {
		auto& viewport = Application::getModule<Vp::Viewport>();

		return viewport.getSettings().scene().mainScene.camera;
	};

	viewport["set_settings"] = [](Vp::CameraSettings& settings) {
		auto& viewport = Application::getModule<Vp::Viewport>();

		viewport.getSettings().scene().mainScene.camera = settings;
		auto mainScene = viewport.getMainScene().lock();
		mainScene->loadSettings(viewport.getSettings(), false, true);
	};

	viewport["update_settings"] = []() {
		auto& viewport = Application::getModule<Vp::Viewport>();
		auto mainScene = viewport.getMainScene().lock();
		mainScene->loadSettings(viewport.getSettings(), false, true);
	};
};
