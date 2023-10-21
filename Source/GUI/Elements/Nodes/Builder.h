/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string_view>

#include "imgui.h"

#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "WorkspaceOperator.h"
#include "WorkspaceTransformation.h"
#include "WorkspaceTransformation_s.h"

class OperatorBuilder
{
public:
	OperatorBuilder() noexcept
	{
		constexpr std::size_t maxCount = magic_enum::enum_count<Core::EOperatorType>() - 1;
		doFor<0, maxCount>();
	}

	std::optional<Ptr<WorkspaceNodeWithCoreData>> operator()(const char* type)
	{
		I3T_ASSERT(m_createFns.count(type) == 1, "Cannot find factory function for given operator");
		return m_createFns.at(type)(ImVec2(0.0f, 0.0f));
	}

private:
	template <int N, int Max> constexpr void doFor() noexcept
	{
		constexpr auto enumValue = static_cast<Core::EOperatorType>(N);
		m_createFns[magic_enum::enum_name(enumValue)] = addNodeToNodeEditorNoSave<WorkspaceOperator<enumValue>>;

		if constexpr (N < Max)
		{
			doFor<N + 1, Max>();
		}
	}

	std::map<std::string_view, std::function<Ptr<WorkspaceNodeWithCoreData>(ImVec2 const)>> m_createFns;
};

//

class TransformBuilder
{
public:
	TransformBuilder() noexcept
	{
		constexpr std::size_t maxCount = magic_enum::enum_count<Core::ETransformType>() - 1;
		doFor<0, maxCount>();
	}

	std::optional<Ptr<WorkspaceTransformation>> operator()(const char* type)
	{
		I3T_ASSERT(m_createFns.count(type) == 1, "Cannot find factory function for given transform");
		return std::static_pointer_cast<WorkspaceTransformation>(m_createFns.at(type)(ImVec2(0.0f, 0.0f)));
	}

private:
	template <int N, int Max> constexpr void doFor() noexcept
	{
		constexpr auto enumValue = static_cast<Core::ETransformType>(N);
		m_createFns[magic_enum::enum_name(enumValue)] = addNodeToNodeEditorNoSave<WorkspaceTransformation_s<enumValue>>;

		if constexpr (N < Max)
		{
			doFor<N + 1, Max>();
		}
	}

	std::map<std::string_view, std::function<Ptr<WorkspaceNodeWithCoreData>(ImVec2 const)>> m_createFns;
};
