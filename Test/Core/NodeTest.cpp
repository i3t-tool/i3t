#include "gtest/gtest.h"

#include "Core/Node.h"
#include "Core/NodeBuilder.h"

using namespace Core;

TEST(PlugTwoVec3Nodes, TheirDotProductShouldBeCorrect)
{
  auto vec1 = Builder::createNode<ENodeType::Vector3>();
  auto vec2 = Builder::createNode<ENodeType::Vector3>();
  auto dotNode = Builder::createNode<ENodeType::Vector3DotVector3>();

  // Plug vec1 and vec2 to dotNode inputs.
  dotNode->plugToParent(vec1, 0, 0);
  dotNode->plugToParent(vec2, 0, 1);

  vec1->getInternalData().setValue(glm::vec3(1.0f, 2.0f, 1.0f));
  vec2->getInternalData().setValue(glm::vec3(2.0f, 1.0f, 2.0f));
  dotNode->updateValues(0);

  EXPECT_EQ(6.0f, dotNode->getInternalData().getFloat());
}

TEST(PlugTwoNodesWithDifferentTypes, NodeShouldNotBeConnected)
{
  auto fl = Builder::createNode<ENodeType::Float>();
  auto vec3 = Builder::createNode<ENodeType::Vector3>();

  auto result = fl->plugToParent(vec3, 0, 0);

  EXPECT_EQ(ENodePlugResult::Err_MismatchedPinTypes, result);
}

TEST(PlugNodeOuputToTheSameNodeInput, NodeShouldNotBeConnected)
{
  auto vec3 = Builder::createNode<ENodeType::Vector3>();

  auto result = vec3->plugToParent(vec3, 0, 0);

  EXPECT_EQ(ENodePlugResult::Err_Loopback, result);
}

TEST(PlugWouldCreateCycleInGraph, NodesShouldNotBeConnected)
{
  auto f1 = Builder::createNode<ENodeType::Float>();
  auto f2 = Builder::createNode<ENodeType::Float>();
  auto f3 = Builder::createNode<ENodeType::Float>();
  auto f4 = Builder::createNode<ENodeType::Float>();

  f1->plugToParent(f2, 0, 0);
  f2->plugToParent(f3, 0, 0);
  f3->plugToParent(f4, 0, 0);

  auto result = f4->plugToParent(f1, 0, 0);

  EXPECT_EQ(ENodePlugResult::Err_Loop, result);
}

TEST(SimpleUnplug, UnplugMeFromPluggedNode)
{
  auto vec1 = Builder::createNode<ENodeType::Vector3>();
  auto vec2 = Builder::createNode<ENodeType::Vector3>();

  vec2->plugToParent(vec1, 0, 0);

  vec1->getInternalData().setValue(glm::vec3(2.1f, 0.5f, 0.0f));
  vec2->updateValues(0);

  EXPECT_FALSE(vec2->getInternalData().getVec3().x == 0.0f);

  vec1->unplugAll();

  const auto& outputPins = vec1->getOutputPins();
  EXPECT_TRUE(outputPins[0].getOutComponents().empty());
}

TEST(UnplugInTheMiddleOfChain, UnplugShouldBeSuccessful)
{
  auto vec1 = Builder::createNode<ENodeType::Vector3>();
  auto vec2 = Builder::createNode<ENodeType::Vector3>();
  auto vec3 = Builder::createNode<ENodeType::Vector3>();

  vec2->plugToParent(vec1, 0, 0);
  vec3->plugToParent(vec2, 0, 0);

  vec2->unplugAll();

  const auto& vec1OutputPins = vec1->getOutputPins();
  EXPECT_TRUE(vec1OutputPins[0].getOutComponents().empty());

  EXPECT_FALSE(vec2->getInputPins()[0].isPluggedIn());
  EXPECT_TRUE(vec2->getOutputPins()[0].getOutComponents().empty());

  EXPECT_FALSE(vec3->getInputPins()[0].isPluggedIn());
}
