#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "../Utils.h"
#include "Generator.h"

using namespace Core;

/**
 * Float node should contain <vec3|vec3> value.
 *
 * vec3 --- dot --- float
 *           |
 *          vec3
 */
TEST(SpreadSignalTest, ValuesShouldBeSpreadThroughConnectedNodes)
{
	auto vec1Node = Core::Builder::createOperator<ENodeType::Vector3ToVector3>();
	auto vec2Node = Core::Builder::createOperator<ENodeType::Vector3ToVector3>();
	auto dotNode = Core::Builder::createOperator<ENodeType::Vector3DotVector3>();
	auto floatNode = Core::Builder::createOperator<ENodeType::FloatToFloat>();

	// Plug vec1 and vec2 to dotNode inputs.
	plug_expectOk(vec1Node, dotNode, 0, 0);
	plug_expectOk(vec2Node, dotNode, 0, 1);
	plug_expectOk(dotNode, floatNode, 0, 0);

	auto vec1 = generateVec3();
	auto vec2 = generateVec3();

	setValue_expectOk(vec1Node, vec1);
	setValue_expectOk(vec2Node, vec2);

	float expectedValue = glm::dot(vec1, vec2);
	EXPECT_EQ(expectedValue, floatNode->getData().getFloat());
}
