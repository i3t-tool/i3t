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

#include <map>
#include <sstream>

#include "glm/glm.hpp"

#include "Core/Format.h"
#include "Core/Nodes/NodeData.h"

namespace Utils
{
inline std::string toString(const fs::path& path)
{
	auto result = path.string();

	if (fs::path::preferred_separator == '\\')
	{
		std::replace(result.begin(), result.end(), '\\', '/');
	}

	return result;
}

inline std::string toString(float fl)
{
	std::string str;

	if (std::isnan(fl))
		str = fl < 0.0f ? "-.NaN" : ".NaN";
	else
		str = fmt::format("{}", fl);

	return str;
}

inline std::string toString(const glm::vec3& vec)
{
	return fmt::format("[{}, {}, {}]", toString(vec.x), toString(vec.y), toString(vec.z));
}

inline std::string toString(const glm::vec4& vec)
{
	return fmt::format("[{}, {}, {}, {}]", toString(vec.x), toString(vec.y), toString(vec.z), toString(vec.w));
}

inline std::string toString(const glm::quat& quat)
{
	return fmt::format("[{}, ({}, {}, {})]", toString(quat.w), toString(quat.x), toString(quat.y), toString(quat.z));
}

inline std::string toString(const glm::mat4& mat, bool breakLines = false)
{
	if (breakLines)
		return fmt::format("[{}, \n{}, \n{}, \n{}]", toString(mat[0]), toString(mat[1]), toString(mat[2]),
		                   toString(mat[3]));

	return fmt::format("[{}, {}, {}, {}]", toString(mat[0]), toString(mat[1]), toString(mat[2]), toString(mat[3]));
}

inline std::string concat(const std::vector<std::string>& entries, const std::string& delimiter)
{
	std::stringstream ss;
	const auto entriesSize = entries.size();
	for (size_t i = 0; i < entriesSize; ++i)
	{
		if (i != 0)
			ss << delimiter;
		ss << entries[i];
	}
	return ss.str();
}

inline std::vector<std::string> concat(const std::map<std::string, std::string>& map, const std::string& delimiter)
{
	std::vector<std::string> vec;

	for (const auto& [key, val] : map)
		vec.push_back(fmt::format("{}{}{}", key, delimiter, val));

	return vec;
}

std::vector<std::string> concat(const std::map<std::string, Core::Data>& map, const std::string& delimiter);
} // namespace Utils
