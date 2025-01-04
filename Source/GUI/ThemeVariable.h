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

#include "imgui.h"

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
	bool forceInt = false;
};

struct ThemeGroup
{
	const char* name;
	std::vector<ThemeVariable> variables;

	/// \see ThemeVariable parameters
	template <typename... Args>
	ThemeGroup& add(Args&&... args)
	{
		variables.emplace_back(std::forward<Args>(args)...);

		return *this;
	}
};
