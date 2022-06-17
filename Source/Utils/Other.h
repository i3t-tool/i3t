#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "Core/Defs.h"

namespace String
{
inline bool contains(const std::string& str, const std::string& substr)
{
	size_t index;

	// Check if window can be focused (no menu is active).
	if ((index = str.find(substr, 0L)) != std::string::npos)
	{
		if (index == 0) { return true; }
	}
	return false;
}
} // namespace String

namespace Utils
{
template <typename A, typename Predicate>
inline int indexOf(A&& array, Predicate p)
{
	auto it = std::find_if(array.begin(), array.end(), p);
	if (it != array.end()) { return std::distance(array.begin(), it); }
	return -1;
}
} // namespace Utils


/**
 * Convert enum to integer (or any other underlying type
 * https://en.cppreference.com/w/cpp/types/underlying_type).
 * \tparam E enum type
 * \param e enum instance
 * \return enum underlying type.
 */
template <typename E>
constexpr auto toUnderlying(E e) noexcept
{
	return static_cast<std::underlying_type_t<E>>(e);
}

struct COutRedirect
{
	COutRedirect() { m_default = std::cout.rdbuf(m_buffer.rdbuf()); }

	~COutRedirect() { std::cout.rdbuf(m_default); }

	std::stringstream& GetBuffer() { return m_buffer; }

	std::string GetStr() { return m_buffer.str(); }

private:
	std::stringstream m_buffer;
	std::streambuf*   m_default;
};
