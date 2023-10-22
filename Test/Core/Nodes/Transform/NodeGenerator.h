/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <tuple>

#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"

using namespace Core;

/// Generate and connect float inputs to given node.
template <size_t Size>
inline std::tuple<std::array<float, Size>, std::array<Ptr<Node>, Size>> generateFloatInputs(Ptr<Core::Node> node)
{
	std::array<float, Size> inputValues;
	std::array<Ptr<Core::Node>, Size> inputNodes;

	for (unsigned int i = 0; i < Size; ++i)
	{
		inputValues[i] = generateFloat();
		inputNodes[i] = Builder::createOperator<EOperatorType::FloatToFloat>();
		auto valueSetResult = inputNodes[i]->setValue(inputValues[i]);
		auto plugResult = GraphManager::plug(inputNodes[i], node, 0, i);
		EXPECT_EQ(ValueSetResult::Status::Ok, valueSetResult.status);
		EXPECT_EQ(ENodePlugResult::Ok, plugResult);
	}
	return std::make_tuple(inputValues, inputNodes);
}
