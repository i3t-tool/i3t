#pragma once

#include <tuple>

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"

using namespace Core;

/// Generate and connect float inputs to given node.
template <size_t Size>
std::tuple<std::array<float, Size>, std::array<Ptr<NodeBase>, Size>> generateFloatInputs(Ptr<Core::NodeBase> node)
{
	std::array<float, Size> inputValues;
	std::array<Ptr<Core::NodeBase>, Size> inputNodes;

	for (size_t i = 0; i < Size; ++i)
	{
		inputValues[i] = generateFloat();
		inputNodes[i] = Builder::createNode<ENodeType::Float>();
		inputNodes[i]->setValue(inputValues[i]);
		GraphManager::plug(inputNodes[i], node, 0, i);
	}

	return std::make_tuple(inputValues, inputNodes);
}
