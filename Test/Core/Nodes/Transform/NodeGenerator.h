#pragma once

#include <tuple>

#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"

using namespace Core;

/// Generate and connect float inputs to given node.
template <size_t Size>
inline std::tuple<std::array<float, Size>, std::array<Ptr<Node>, Size>>
generateFloatInputs(Ptr<Core::Node> node)
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
