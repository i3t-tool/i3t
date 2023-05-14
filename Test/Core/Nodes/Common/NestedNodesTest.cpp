#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

TEST(NestedNodesTest, _)
{
	auto seq = GraphManager::createSequence();

	EXPECT_EQ(seq->getId(), seq->getInput(0).getOwner()->getId());
}
