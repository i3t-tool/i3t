#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

TEST(PinViewTest, RangeFor)
{
	auto node = Core::Builder::createNode<ENodeType::Vector3ToVector3>();

	auto outputs = node->getOutputPins();
	auto begin = outputs.begin();
	auto end = outputs.end();

	int i = 0;

	for (; begin != end; ++begin)
	{
		auto val = *begin;
		i++;
	}

	EXPECT_EQ(i, node->getOperation()->outputTypes.size());
}
