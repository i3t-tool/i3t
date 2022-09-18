#pragma once

#include <string>

inline bool ends_with(std::string const& value, std::string const& ending)
{
	if (ending.size() > value.size())
		return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}
