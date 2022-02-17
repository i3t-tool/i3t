#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

TEST(ValueStateTest, _)
{
	auto vec3 = GraphManager::createNode<ENodeType::Vector3ToVector3>();
	EXPECT_EQ(vec3->getState(0), EValueState::Editable);

	auto tmp = GraphManager::createNode<ENodeType::Vector3ToVector3>();
	GraphManager::plug(tmp, vec3, 0, 0);
	EXPECT_EQ(vec3->getState(0), EValueState::Locked);

	GraphManager::unplugAll(tmp);
	EXPECT_EQ(vec3->getState(0), EValueState::Editable);
}
