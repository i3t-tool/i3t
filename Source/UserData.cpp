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
#include "UserData.h"

#include <algorithm>

#include "rttr/registration.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<UserData>("UserData")
	    .property("themeName", &UserData::customThemeName)
	    .property("recentFiles", &UserData::recentFiles);
}

void UserData::pushRecentFile(const std::filesystem::path& file)
{
	const auto it = std::find(recentFiles.begin(), recentFiles.end(), file);
	if (it != recentFiles.end())
	{
		recentFiles.erase(it);
	}

	recentFiles.push_back(file);
}

UserData& getUserData()
{
	static UserData data;
	return data;
}
