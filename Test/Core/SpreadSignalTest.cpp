#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

TEST(SetValueOnVector, SignalSpreadedThroughAllConnectedNodes)
{
  auto vec1 = Builder::createNode<ENodeType::Vector3>();
  auto vec2 = Builder::createNode<ENodeType::Vector3>();
  auto dotNode = Builder::createNode<ENodeType::Vector3DotVector3>();
	auto floatNode = Builder::createNode<ENodeType::Float>();

  // Plug vec1 and vec2 to dotNode inputs.
  GraphManager::plug(vec1, dotNode, 0, 0);
  GraphManager::plug(vec2, dotNode, 0, 1);
	GraphManager::plug(dotNode, floatNode, 0, 0);

	vec1->setValue(glm::vec3(1.0f, 2.0f, 3.0f));
	vec2->setValue(glm::vec3(3.0f, 2.0f, -1.0f));

	EXPECT_EQ(4.0f, floatNode->getData().getFloat());
}
