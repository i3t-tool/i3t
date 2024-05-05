#include "Utils/FilesystemUtils.h"

#include <fstream>

namespace FilesystemUtils
{

Result<std::string, Error> readFile(const fs::path& path)
{
	try
	{
		std::ifstream file(path);
		if (!file.is_open())
		{
			return Err("Could not open file: " + path.string());
		}
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		return content;
	}
	catch (const std::exception& e)
	{
		return Err("Filesystem error: " + std::string(e.what()));
	}
}

} // namespace FilesystemUtils
