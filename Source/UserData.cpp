#include "UserData.h"

#include <algorithm>

#include "rttr/registration.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<UserData>("UserData")
	    .property("themeName", &UserData::themeName)
	    .property("recentFiles", &UserData::recentFiles);
}

void UserData::pushRecentFile(const std::string& file)
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
