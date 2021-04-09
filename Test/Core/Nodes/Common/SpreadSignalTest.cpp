#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "../Utils.h"

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
  auto vec1Node = Core::Builder::createNode<ENodeType::Vector3>();
  auto vec2Node = Core::Builder::createNode<ENodeType::Vector3>();
  auto dotNode = Core::Builder::createNode<ENodeType::Vector3DotVector3>();
	auto floatNode = Core::Builder::createNode<ENodeType::Float>();

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
