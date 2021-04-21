#pragma once

#include <tuple>

#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"

using namespace Core;

/// Generate and connect float inputs to given node.
template <size_t Size>
inline std::tuple<std::array<float, Size>, std::array<Ptr<NodeBase>, Size>> generateFloatInputs(Ptr<Core::NodeBase> node)
{
	std::array<float, Size> inputValues;
	std::array<Ptr<Core::NodeBase>, Size> inputNodes;

	for (size_t i = 0; i < Size; ++i)
	{
		inputValues[i] = generateFloat();
		inputNodes[i] = Builder::createNode<ENodeType::Float>();
		auto valueSetResult = inputNodes[i]->setValue(inputValues[i]);
		auto plugResult = GraphManager::plug(inputNodes[i], node, 0, i);
		EXPECT_EQ(ValueSetResult::Status::Ok, valueSetResult.status);
		EXPECT_EQ(ENodePlugResult::Ok, plugResult);
	}
	return std::make_tuple(inputValues, inputNodes);
}
