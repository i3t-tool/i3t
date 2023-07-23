#pragma once

#include <string>
#include <vector>

constexpr auto USER_DATA_DIR = "UserData";
constexpr auto USER_DATA_FILE = "UserData/Global.json";

/// This struct is reflected by RTTR \see UserData.cpp file.
struct UserData
{
	std::string themeName = "classic";

	/// Last file is the most recent.
	std::vector<std::string> recentFiles;

	void pushRecentFile(const std::string& file);
};

UserData& getUserData();
