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

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "GUI/Workspace/Nodes/Operator.h"
#include "GUI/Workspace/Nodes/Transformation.h"
#include "GUI/Workspace/Nodes/TransformationBase.h"

namespace Workspace
{
class OperatorBuilder
{
public:
	OperatorBuilder() noexcept
	{
		constexpr std::size_t maxCount = magic_enum::enum_count<Core::EOperatorType>() - 1;
		doFor<0, maxCount>();
	}

	std::optional<Ptr<CoreNodeWithPins>> operator()(const char* type)
	{
		I3T_ASSERT(m_createFns.count(type) == 1, "Cannot find factory function for given operator");
		return m_createFns.at(type)(ImVec2(0.0f, 0.0f));
	}

private:
	template <int N, int Max>
	constexpr void doFor() noexcept
	{
		constexpr auto enumValue = static_cast<Core::EOperatorType>(N);
		m_createFns[magic_enum::enum_name(enumValue)] = addNodeToNodeEditorNoSave<Operator<enumValue>>;

		if constexpr (N < Max)
		{
			doFor<N + 1, Max>();
		}
	}

	std::map<std::string_view, std::function<Ptr<CoreNodeWithPins>(ImVec2 const)>> m_createFns;
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

	std::optional<Ptr<TransformationBase>> operator()(const char* type)
	{
		I3T_ASSERT(m_createFns.count(type) == 1, "Cannot find factory function for given transform");
		return std::static_pointer_cast<TransformationBase>(m_createFns.at(type)(ImVec2(0.0f, 0.0f)));
	}

private:
	template <int N, int Max>
	constexpr void doFor() noexcept
	{
		constexpr auto enumValue = static_cast<Core::ETransformType>(N);
		m_createFns[magic_enum::enum_name(enumValue)] = addNodeToNodeEditorNoSave<Transformation<enumValue>>;

		if constexpr (N < Max)
		{
			doFor<N + 1, Max>();
		}
	}

	std::map<std::string_view, std::function<Ptr<CoreNode>(ImVec2 const)>> m_createFns;
};
} // namespace Workspace