#include "Format.h"

namespace Utils
{
std::vector<std::string> /*Utils::*/concat(const std::map<std::string, DataStore>& map, const std::string& delimiter)
{
	std::vector<std::string> vec;
	for (const auto& [key, val] : map)
	{
		std::string strVal = "undefined";
		switch (val.opValueType)
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
}
