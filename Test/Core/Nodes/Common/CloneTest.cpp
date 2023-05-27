#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "../Utils.h"
#include "Generator.h"

using namespace Core;

TEST(CloneNodeTest, NodeCanBeCloned)
{
	auto node = Builder::createOperator<EOperatorType::FloatToFloat>();

	const float expectedValue = generateFloat();
	setValue_expectOk(node, expectedValue);

	auto cloned = node->clone();

	EXPECT_EQ(expectedValue, cloned->getData().getFloat());
}

TEST(CloneNodeTest, TransformCanBeCloned)
{
	auto transform = Builder::createTransform<ETransformType::Translation>();

	const auto expectedValue = generateVec3();
	transform->setDefaultValue("translation", expectedValue);

	auto cloned = transform->clone();

	EXPECT_EQ(expectedValue, cloned->as<Transform>()->getDefaultValue("translation").getVec3());
	EXPECT_TRUE(Math::eq(transform->getData().getMat4(), cloned->getData().getMat4()));
}
