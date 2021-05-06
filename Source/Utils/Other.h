#include <iostream>
#include <string>
#include <type_traits>

#include "Core/Defs.h"

/**
 * Convert enum to integer (or any other underlying type
 * https://en.cppreference.com/w/cpp/types/underlying_type).
 * \tparam E enum type
 * \param e enum instance
 * \return enum underlying type.
 */
template <typename E> constexpr auto toUnderlying(E e) noexcept
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
  std::streambuf* m_default;
};

namespace FS
{
// Only for debug purposes.
FORCE_INLINE std::string absolute(const std::string& relativePath)
{
	return I3T_PROJECT_ROOT + relativePath;
}
} // namespace FS
