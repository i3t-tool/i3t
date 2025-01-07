/**
 * \file
 * \brief A bunch of useful definitions and shortcuts.
 * \author Martin Herich <martin.herich@phire.cz>
 * \date 24.10.2020
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>

#include "magic_enum.hpp"

#ifndef EMSCRIPTEN
#include "spdlog/formatter.h"
#include "spdlog/fmt/fmt.h"
#else
namespace fmt
{
template <typename T, typename... Args>
std::string format(const T& str, Args&&... args)
{
	return str;
}
}
#endif

#include "Core/Types.h"
#include "Logger/Logger.h"

namespace fs = std::filesystem;

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

/// We need to log message here since all assertions are removed
/// on Windows in Release config.
#define I3T_ASSERT(cond, message)                                                                                      \
	if (!(cond))                                                                                                       \
	{                                                                                                                  \
		LOG_FATAL(message);                                                                                            \
	}                                                                                                                  \
	assert(cond)
#define I3T_ABORT(message)                                                                                             \
	LOG_FATAL(message);                                                                                                \
	std::abort()

/// Singleton
template <typename T>
class Singleton
{
public:
	static T& instance()
	{
		static T instance;
		return instance;
	}
};

/// Enum utils

/**
 * Get string name from enum value.
 * \tparam T enum definition (such as EOperatorType or ETransformType)
 * \param val Enum value
 * \return String name of the enum value
 */
template <typename T>
auto n(T val)
{
	return std::string(magic_enum::enum_name(val));
}

namespace EnumUtils
{
template <typename T>
auto name(T val)
{
	return n(val);
}

template <typename T>
std::optional<T> value(const std::string& str)
{
	return magic_enum::enum_cast<T>(str);
}

template <typename T>
std::optional<T> value(std::string_view str)
{
	return magic_enum::enum_cast<T>(str);
}
} // namespace EnumUtils

#define COND_TO_DEG(x)                                                                                                 \
	(SetupForm::radians ? (x) : glm::degrees(x)) ///< Converts from radians to degrees if the
	                                             ///< application set up for degrees
