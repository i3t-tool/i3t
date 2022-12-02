#pragma once

#include <algorithm>
#include <string>

inline bool ends_with(std::string const& value, std::string const& ending)
{
	if (ending.size() > value.size())
		return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

template <typename T> inline void hash_combine(std::size_t& seed, const T& v)
{
	seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename It> inline std::size_t hash_range(It first, It last)
{
	std::size_t seed = 0;
	for (; first != last; ++first)
	{
		hash_combine(seed, *first);
	}
	return seed;
}

inline size_t hash_string(const std::string& a, const std::string& b,
                          const std::string& c)
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