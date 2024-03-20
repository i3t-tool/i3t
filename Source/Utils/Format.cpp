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
#include "Format.h"

namespace Utils
{
std::vector<std::string> concat(const std::map<std::string, Core::Data>& map, const std::string& delimiter)
{
	using namespace Core;

	std::vector<std::string> vec;
	for (const auto& [key, val] : map)
	{
		std::string strVal = "undefined";
		switch (val.valueType)
		{
		case EValueType::Float:
			strVal = toString(val.getFloat());
			break;
		case EValueType::Vec3:
			strVal = toString(val.getVec3());
			break;
		case EValueType::Vec4:
			strVal = toString(val.getVec4());
			break;
		case EValueType::Matrix:
			strVal = toString(val.getMat4());
			break;
		case EValueType::Quat:
			strVal = toString(val.getQuat());
			break;
		}
		vec.push_back(fmt::format("{}{}{}", key, delimiter, strVal));
	}
	return vec;
}
} // namespace Utils
