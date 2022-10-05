/**
 * \file Core/Defs.h
 * \author Martin Herich
 * \date 24.10.2020
 *
 * A bunch of useful definitions and shortcuts.
 */
#pragma once

#include <cassert>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "magic_enum.hpp"
#include "spdlog/formatter.h"

namespace fs = std::filesystem;

constexpr const size_t MAX_PATH_LENGTH = 4096L;

#define I3T_CONST inline constexpr

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

#ifdef Assert
#undef Assert /* \todo JH due to compile error */
#endif

#ifdef I3T_DEBUG
#define I3T_ASSERT(cond) assert(cond)
#define I3T_ABORT(message) assert(false && message)
#else
#define I3T_ASSERT(cond, description)
#endif

namespace Debug
{
template <typename... Args>
void Assert(bool condition, const std::string& message = "", Args&&... args)
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

template <typename T> using WPtr = std::weak_ptr<T>;

/// Singleton helper class.
template <typename T> class Singleton
{
public:
	static T& instance();

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton) = delete;

protected:
	Singleton() {}

	void release();

	inline static std::unique_ptr<T> m_instance = nullptr;
};

template <typename T> T& Singleton<T>::instance()
{
	if (m_instance == nullptr)
		m_instance = std::make_unique<T>();

	return *m_instance;
}

template <typename T> void Singleton<T>::release()
{
	m_instance.reset(new T{});
}

//

template <typename BaseClass> class ICloneable
{
public:
	virtual Ptr<BaseClass> clone() = 0;
};

/// Enum utils

/**
 * Get string name from enum value.
 * \tparam T enum definition (such as ENodeType or ETransformType)
 * \param val Enum value
 * \return String name of the enum value
 */
template <typename T> auto n(T val)
{
	return std::string(magic_enum::enum_name(val));
}

namespace EnumUtils
{
template <typename T> auto name(T val) { return n(val); }

template <typename T> std::optional<T> value(const std::string& str)
{
	return magic_enum::enum_cast<T>(str);
}
} // namespace EnumUtils

#define COND_TO_DEG(x)                                                         \
	(SetupForm::radians                                                          \
	     ? (x)                                                                   \
	     : glm::degrees(x)) ///< Converts from radians to degrees if the
	                        ///< application set up for degrees
