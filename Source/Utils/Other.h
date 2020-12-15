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
template <typename E>
constexpr auto toUnderlying(E e) noexcept
{
  return static_cast<std::underlying_type_t<E>>(e);
}

namespace FS
{
FORCE_INLINE std::string absolute(const std::string& relativePath)
{
  return I3T_PROJECT_ROOT + relativePath;
}
}
