/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <filesystem>
#include <string>
#include <vector>

constexpr auto USER_DATA_DIR = "UserData";
constexpr auto USER_DATA_FILE = "UserData/Global.json";

/// This struct is reflected by RTTR \see UserData.cpp file.
struct UserData
{
	/// If empty, LightMode or DarkMode is used based on system settings.
	std::string customThemeName;

	/// Last file is the most recent.
	std::vector<std::filesystem::path> recentFiles;

	void pushRecentFile(const std::filesystem::path& file);
};

UserData& getUserData();
