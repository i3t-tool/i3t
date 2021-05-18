#pragma once

#include <filesystem>

#include <sys/stat.h>

namespace fs = std::filesystem;

inline bool doesFileExists(const char* filename)
{
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}

inline bool doesFileExists(const std::string& path)
{
	return doesFileExists(path.c_str());
}

inline bool doesFileExists(fs::path& path)
{
	return doesFileExists(path.string().c_str());
}
