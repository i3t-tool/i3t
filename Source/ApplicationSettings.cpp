/**
 * \file ApplicationSettings.cpp
 * \brief
 * \author Matvei Korikov <korikmat@fel.cvut.cz>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * Licensed under the GNU General Public License v3.0 (see LICENSE.txt or
 * https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#include "ApplicationSettings.h"

#include "rttr/registration.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<WindowSettings>("WindowSettings")
	    .property("windowMaximized", &WindowSettings::windowMaximized)
	    .property("windowWidth", &WindowSettings::windowWidth)
	    .property("windowHeight", &WindowSettings::windowHeight)
	    .property("windowPosX", &WindowSettings::windowPosX)
	    .property("windowPosY", &WindowSettings::windowPosY);

	rttr::registration::class_<AppLoopSettings>("AppLoopSettings")
	    .property("vsync", &AppLoopSettings::vsync)
	    .property("shouldLimitFPS", &AppLoopSettings::shouldLimitFPS)
	    .property("targetFPS", &AppLoopSettings::targetFPS)
	    .property("shouldLimitFPSOnIdle", &AppLoopSettings::shouldLimitFPSOnIdle)
	    .property("targetFPSOnIdle", &AppLoopSettings::targetFPSOnIdle);

	rttr::registration::class_<ApplicationSettings>("ApplicationSettings")
	    .property("appLoopSettings", &ApplicationSettings::appLoopSettings)
	    .property("windowSettings", &ApplicationSettings::windowSettings);
}
