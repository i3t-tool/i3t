/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "../../../Dependencies/imgui/imgui.h"

#include <optional>
#include <variant>
#include <vector>

enum class EColor;
enum class ESize;
enum class ESizeVec2;

using ThemeVariableKey = std::variant<EColor, ESize, ESizeVec2>;

struct ThemeVariable
{
	ThemeVariableKey key;
	const char* name = nullptr;
	const char* description = nullptr;
	std::optional<ImVec2> range;
	float speed;
	const char* format;
};

struct ThemeGroup
{
	const char* name;
	const char* id;
	int indent = 0;

	std::vector<ThemeVariable> variables;

	ThemeGroup(const char* name, const char* id, int indent) : name(name), id(id), indent(indent) {}

	/// Add a theme variable to the group, restriced to positive values.
	/// Default speed and format essentially emulates an int variable
	ThemeGroup& add(const ThemeVariableKey&& key, const char* name, const char* desc = nullptr,
	                const std::optional<ImVec2>& range = ImVec2(0.0f, FLT_MAX), float speed = 1.0f,
	                const char* format = "%.0f")
	{
		variables.emplace_back(key, name, desc, range, speed, format);
		return *this;
	}

	/// Add a theme variable to the group, restriced to <1, Inf) interval.
	/// @see add()
	ThemeGroup& add1(const ThemeVariableKey&& key, const char* name, const char* desc = nullptr,
	                 const std::optional<ImVec2>& range = ImVec2(1.0f, FLT_MAX), float speed = 1.0f,
	                 const char* format = "%.0f")
	{
		return add(std::move(key), name, desc, range, speed, format);
	}

	/// Add a theme variable to the group, unrestricted range.
	/// @see add()
	ThemeGroup& addN(const ThemeVariableKey&& key, const char* name, const char* desc = nullptr,
	                 const std::optional<ImVec2>& range = std::nullopt, float speed = 1.0f, const char* format = "%.0f")
	{
		return add(std::move(key), name, desc, range, speed, format);
	}
};
