#include "gtest/gtest.h"

#include "Generator.h"
#include "Core/Nodes/GraphManager.h"

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
  GraphManager::plug(vec1Node, dotNode, 0, 0);
  GraphManager::plug(vec2Node, dotNode, 0, 1);
	GraphManager::plug(dotNode, floatNode, 0, 0);

	auto vec1 = generateVec3();
	auto vec2 = generateVec3();

  {
    auto result = vec1Node->setValue(vec1);
    EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
	}
  {
    auto result = vec2Node->setValue(vec2);
    EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
  }

	float expectedValue = glm::dot(vec1, vec2);
	EXPECT_EQ(expectedValue, floatNode->getData().getFloat());
}
