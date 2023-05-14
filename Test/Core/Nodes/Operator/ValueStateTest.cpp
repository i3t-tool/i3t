#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

TEST(ValueStateTest, _)
{
	auto leftNode = GraphManager::createNode<ENodeType::Vector3ToVector3>();
	EXPECT_EQ(leftNode->getState(0), EValueState::Editable);

	auto rightNode = GraphManager::createNode<ENodeType::Vector3ToVector3>();
	GraphManager::plug(leftNode, rightNode, 0, 0);
	EXPECT_EQ(rightNode->getState(0), EValueState::Locked);

	GraphManager::unplugAll(rightNode);
	EXPECT_EQ(rightNode->getState(0), EValueState::Editable);
}
