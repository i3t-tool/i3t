/**
 * \file ApplicationSettings.h
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
#pragma once

struct AppLoopSettings
{
	bool vsync{true};
	bool shouldLimitFPS{false};
	int targetFPS{60};

	bool shouldLimitFPSOnIdle = true;
	int targetFPSOnIdle{20};
};

struct WindowSettings
{
	bool windowMaximized{false};
	int windowWidth{1280};
	int windowHeight{720};
	int windowPosX{100};
	int windowPosY{100};
};

struct ApplicationSettings
{
	AppLoopSettings appLoopSettings;
	WindowSettings windowSettings;

	AppLoopSettings& appLoop()
	{
		return appLoopSettings;
	}
	WindowSettings& window()
	{
		return windowSettings;
	}
};
