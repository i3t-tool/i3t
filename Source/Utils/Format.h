#pragma once

#include "spdlog/fmt/fmt.h"

namespace Utils
{
std::string toString(const glm::vec3& vec)
{
	return fmt::format("[{}, {}, {}]", vec.x, vec.y, vec.z);
}

std::string toString(const glm::vec4& vec)
{
	return fmt::format("[{}, {}, {}, {}]", vec.x, vec.y, vec.z, vec.w);
}

std::string toString(const glm::quat& quat)
{
	return fmt::format("[{}, {}, {}, {}]", quat.x, quat.y, quat.z, quat.w);
}

std::string toString(const glm::mat4& mat)
{
	return fmt::format("[{}, {}, {}, {}]", toString(mat[0]), toString(mat[1]), toString(mat[2]), toString(mat[3]));
}

std::string concat(const std::vector<std::string>& entries, const std::string delimiter)
{
	std::stringstream ss;
	const auto entriesSize = entries.size();
	for(size_t i = 0; i < entriesSize; ++i)
	{
		if(i != 0)
			ss << delimiter;
		ss << entries[i];
	}
	return ss.str();
}
}
