/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <algorithm>
#include <string>

inline bool ends_with(std::string const& value, std::string const& ending)
{
	if (ending.size() > value.size())
		return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

template <typename T>
inline void hash_combine(std::size_t& seed, const T& v)
{
	seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename It>
inline std::size_t hash_range(It first, It last)
{
	std::size_t seed = 0;
	for (; first != last; ++first)
	{
		hash_combine(seed, *first);
	}
	return seed;
}

inline size_t hash_string(const std::string& a, const std::string& b, const std::string& c, const std::string& d)
{
	std::string input[] = {a, b, c, d};
	std::sort(input, input + (sizeof(input) / sizeof(*input)));
	return hash_range(input, input + (sizeof(input) / sizeof(*input)));
}

inline size_t hash_string(const std::string& a, const std::string& b, const std::string& c)
{
	std::string input[] = {a, b, c};
	std::sort(input, input + (sizeof(input) / sizeof(*input)));
	return hash_range(input, input + (sizeof(input) / sizeof(*input)));
}

inline size_t hash_string(const std::string& a, const std::string& b)
{
	return hash_string(a, b, "");
}

inline size_t hash_string(const std::string& a)
{
	return hash_string(a, "", "");
}

namespace Utils
{
#ifdef _WIN32
// Convert a wide Unicode string to an UTF8 string
std::string utf8Encode(const std::wstring& wstr);

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8Decode(const std::string& str);
#endif
} // namespace Utils
