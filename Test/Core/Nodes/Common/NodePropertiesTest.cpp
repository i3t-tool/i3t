#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

TEST(NodePropertiesTest, InvalidMapShouldNotBeSet)
{
	auto projNode = Builder::createTransform<ETransformType::Perspective>();

	projNode->setDataMap(&Transform::g_EulerY);

	EXPECT_EQ(&Transform::g_Perspective, projNode->getDataMap());
}
