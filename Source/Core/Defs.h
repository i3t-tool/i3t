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

static void i3tAssert(bool condition, std::string message, std::string file, unsigned line)
{
  if (!condition)
    throw std::logic_error(std::string(file + ":" + std::to_string(line) + ": " + message).c_str());
}

/// Debug assert macro.
#ifdef _DEBUG
#define I3T_DEBUG_ASSERT(...) i3tAssert(__VA_ARGS__, __FILE__, __LINE__);
#define I3T_DEBUG
#else
#define I3T_DEBUG_ASSERT(...)
#endif

/// Definition of more friendly shared_ptr usage.
template <typename T> using Ptr = std::shared_ptr<T>;

/// Unique pointer shortcut.
template <typename T> using UPtr = std::unique_ptr<T>;

#define COND_TO_DEG(x)  (SetupForm::radians ? (x)  :  glm::degrees(x) ) ///< Converts from radians to degrees if the application set up for degrees
