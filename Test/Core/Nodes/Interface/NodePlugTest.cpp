#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

TEST(PlugTwoVec3Nodes, TheirDotProductShouldBeCorrect)
{
  auto vec1 = Core::Builder::createNode<ENodeType::Vector3>();
  auto vec2 = Core::Builder::createNode<ENodeType::Vector3>();
  auto dotNode = Core::Builder::createNode<ENodeType::Vector3DotVector3>();

  // Plug vec1 and vec2 to dotNode inputs.
  GraphManager::plug(vec1, dotNode, 0, 0);
  GraphManager::plug(vec2, dotNode, 0, 1);

  vec1->setValue(glm::vec3(1.0f, 2.0f, 1.0f));
  vec2->setValue(glm::vec3(2.0f, 1.0f, 2.0f));
  dotNode->updateValues(0);

  EXPECT_EQ(6.0f, dotNode->getData().getFloat());
}

TEST(PlugTwoNodesWithDifferentTypes, NodeShouldNotBeConnected)
{
  auto fl = Core::Builder::createNode<ENodeType::Float>();
  auto vec3 = Core::Builder::createNode<ENodeType::Vector3>();

  auto result = GraphManager::plug(vec3, fl, 0, 0);

  EXPECT_EQ(ENodePlugResult::Err_MismatchedPinTypes, result);
}

TEST(PlugNodeOuputToTheSameNodeInput, NodeShouldNotBeConnected)
{
  auto vec3 = Core::Builder::createNode<ENodeType::Vector3>();

  auto result = GraphManager::plug(vec3, vec3, 0, 0);

  EXPECT_EQ(ENodePlugResult::Err_Loopback, result);
}

TEST(PlugWouldCreateCycleInGraph, NodesShouldNotBeConnected)
{
  auto f1 = Core::Builder::createNode<ENodeType::Float>();
  auto f2 = Core::Builder::createNode<ENodeType::Float>();
  auto f3 = Core::Builder::createNode<ENodeType::Float>();
  auto f4 = Core::Builder::createNode<ENodeType::Float>();

  GraphManager::plug(f2, f1, 0, 0);
  GraphManager::plug(f3, f2, 0, 0);
  GraphManager::plug(f4, f3, 0, 0);

  auto result = GraphManager::plug(f1, f4, 0, 0);

  EXPECT_EQ(ENodePlugResult::Err_Loop, result);
}

TEST(SimpleUnplug, UnplugMeFromPluggedNode)
{
  auto vec1 = Core::Builder::createNode<ENodeType::Vector3>();
  auto vec2 = Core::Builder::createNode<ENodeType::Vector3>();

  GraphManager::plug(vec1, vec2, 0, 0);

  vec1->setValue(glm::vec3(2.1f, 0.5f, 0.0f));
  vec2->updateValues(0);

  EXPECT_FALSE(vec2->getData().getVec3().x == 0.0f);

  GraphManager::unplugAll(vec1);

  const auto& outputPins = vec1->getOutputPins();
  EXPECT_TRUE(outputPins[0].getOutComponents().empty());
}

TEST(UnplugInTheMiddleOfChain, UnplugShouldBeSuccessful)
{
  auto vec1 = Core::Builder::createNode<ENodeType::Vector3>();
  auto vec2 = Core::Builder::createNode<ENodeType::Vector3>();
  auto vec3 = Core::Builder::createNode<ENodeType::Vector3>();

  GraphManager::plug(vec1, vec2, 0, 0);
  GraphManager::plug(vec2, vec3, 0, 0);

  GraphManager::unplugAll(vec2);

  const auto& vec1OutputPins = vec1->getOutputPins();
  EXPECT_TRUE(vec1OutputPins[0].getOutComponents().empty());

  EXPECT_FALSE(vec2->getInputPins()[0].isPluggedIn());
  EXPECT_TRUE(vec2->getOutputPins()[0].getOutComponents().empty());

  EXPECT_FALSE(vec3->getInputPins()[0].isPluggedIn());
}
