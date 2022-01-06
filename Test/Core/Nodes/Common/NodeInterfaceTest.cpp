#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Transform.h"

#include "../Utils.h"

using namespace Core;

float initialRot = glm::radians(60.0f);
auto initialScale = glm::vec3(1.0f, 2.0f, 3.0f);
auto initialTransl = glm::vec3(-1.0f, 2.0f, 5.0f);

Ptr<NodeBase> scale1, scale2, rotX, translation;
Ptr<NodeBase> mul1, mul2, mul3;

/**
 * Build simple node graph.
 *
 * scale1 --- mul1 --- mul2 --- mul3 --- EMPTY
 *             |        |        |
 *            rotX   scale2  translation
 */
Ptr<NodeBase> prepareEnvironment()
{
  scale1 = Builder::createNode<ENodeType::MakeScale>();
  rotX = Builder::createNode<ENodeType::MakeEulerX>();
  scale2 = Builder::createNode<ENodeType::MakeScale>();
  translation = Builder::createNode<ENodeType::MakeTranslation>();

	// Multiplicate matrices using matrix * matrix node. (Sequence is may not be complete yet!)
  mul1 = Builder::createNode<ENodeType::MatrixMulMatrix>();
  mul2 = Builder::createNode<ENodeType::MatrixMulMatrix>();
  mul3 = Builder::createNode<ENodeType::MatrixMulMatrix>();

  plug_expectOk(scale1, mul1, 0, 0);
  plug_expectOk(rotX, mul1, 0, 1);

  plug_expectOk(mul1, mul2, 0, 0);
  plug_expectOk(scale2, mul2, 0, 1);

  plug_expectOk(mul2, mul3, 0, 0);
  plug_expectOk(translation, mul3, 0, 1);

	return mul3;
}

Ptr<NodeBase> getRoot(Ptr<NodeBase> node)
{
	auto parent = GraphManager::getParent(node);
	if (parent == nullptr)
  {
    return node;
	}
  return getRoot(parent);
}

TEST(NodeInterfaceTest, GetParentShouldGiveValidParentNode)
{
	auto seq1 = Builder::createSequence();
	auto seq2 = Builder::createSequence();
	auto seq3 = Builder::createSequence();
	auto seq4 = Builder::createSequence();

	plug_expectOk(seq1, seq2, 0, 0);
	plug_expectOk(seq2, seq3, 0, 0);
	plug_expectOk(seq3, seq4, 0, 0);

	EXPECT_EQ(seq3, gm::getParent(seq4));
	EXPECT_EQ(seq2, gm::getParent(seq3));
	EXPECT_EQ(seq1, gm::getParent(seq2));
	EXPECT_EQ(nullptr, gm::getParent(seq1));
}

TEST(NodeInterfaceTest, GetParentOfNonexistentPin_ShouldReturnNull)
{
	auto seq = Builder::createSequence();

	EXPECT_EQ(nullptr, gm::getParent(seq, 10));
	EXPECT_EQ(nullptr, gm::getParent(seq, -10));
}

TEST(NodeIntefaceTest, GetNodeInputsAndOutputs_OnComplexGraph_ReturnsValidResults)
{
	// Last node is mul3
	auto lastNode = prepareEnvironment();
	auto lastNodeInputs = GraphManager::getAllInputNodes(lastNode);
	EXPECT_EQ(2, lastNodeInputs.size());
	EXPECT_TRUE(GraphManager::getAllOutputNodes(lastNode).empty());

	auto root = getRoot(lastNode);
  auto anotherMatNode1 = Builder::createNode<ENodeType::MatrixAddMatrix>();
  auto anotherMatNode2 = Builder::createNode<ENodeType::MatrixAddMatrix>();
	plug_expectOk(root, anotherMatNode1, 0, 0);
	plug_expectOk(root, anotherMatNode2, 0, 0);

	auto rootOutputs = GraphManager::getAllOutputNodes(root);
	EXPECT_EQ(1 + 2, rootOutputs.size());
	EXPECT_EQ(3, GraphManager::getOutputNodes(root, 0).size());
}

/*
TEST(NodeInterfaceTest, TypeShouldBeDeducedFromOperationType)
{
	auto scale = Core::Builder::createTransform<ETransformType::Scale>();

	auto* expectedOperation = &g_transforms[static_cast<size_t>(ETransformType::Scale)];

	EXPECT_EQ(expectedOperation->first.keyWord, scale->getOperation()->keyWord);
}
 */

TEST(NodeInterfaceTest, GetAllInputNodes_ShouldReturnEmptyArrayWhenNoNodesConnected)
{
	auto seq = Builder::createSequence();

	auto result = GraphManager::getAllInputNodes(seq);

	EXPECT_TRUE(result.empty());
}

TEST(NodeInterfaceTest, GetAllInputNodes_ShouldReturnNodesConnectedMyInputs)
{
	auto inputSeq = Builder::createSequence();
	auto mySeq    = Builder::createSequence();
	auto mat      = Builder::createNode<ENodeType::MatrixToMatrix>();

	plug_expectOk(inputSeq, mySeq, 0, 0);
	plug_expectOk(mat, mySeq, 0, 1);

	auto result = GraphManager::getAllInputNodes(mySeq);

	EXPECT_EQ(2, result.size());
}

TEST(NodeInterfaceTest, GetNodesConnectedToNonexistentOutputPin)
{
	auto seq = Builder::createSequence();

	EXPECT_THROW(gm::getOutputNodes(seq, 10).empty(), std::exception);
}
