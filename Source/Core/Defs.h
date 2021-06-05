/**
 * \file Core/Defs.h
 * \author Martin Herich
 * \date 24.10.2020
 *
 * A bunch of useful definitions and shortcuts.
 */
#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "spdlog/formatter.h"

constexpr const size_t MAX_PATH_LENGTH = 4096L;

/// Inlining macro.
#ifdef _MSC_VER
#define FORCE_INLINE __forceinline
#elif defined(__GNUC__)
#define FORCE_INLINE inline __attribute__((__always_inline__))
#elif defined(__CLANG__)
#if __has_attribute(__always_inline__)
#define FORCE_INLINE inline __attribute__((__always_inline__))
#else
#define FORCE_INLINE inline
#endif
#else
#define FORCE_INLINE inline
#endif

#undef Assert /* \todo JH due to compile error */

namespace Debug
{
template <typename... Args> void Assert(bool condition, const std::string& message = "", Args&&... args)
{
#ifdef I3T_DEBUG
	if (!condition)
		throw std::logic_error(fmt::format(message, std::forward<Args>(args)...));
#endif
}
} // namespace Debug

/// Definition of more friendly shared_ptr usage.
template <typename T> using Ptr = std::shared_ptr<T>;

/// Unique pointer shortcut.
template <typename T> using UPtr = std::unique_ptr<T>;

#define COND_TO_DEG(x)                                                                                               \
	(SetupForm::radians ? (x)                                                                                          \
	                    : glm::degrees(x)) ///< Converts from radians to degrees if the application set up for degrees
