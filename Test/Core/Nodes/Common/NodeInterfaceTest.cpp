/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Screen.h"
#include "Core/Nodes/Transform.h"

#include "../Utils.h"

using namespace Core;

float initialRot = glm::radians(60.0f);
auto initialScale = glm::vec3(1.0f, 2.0f, 3.0f);
auto initialTransl = glm::vec3(-1.0f, 2.0f, 5.0f);

struct TestData
{
	Ptr<Node> scale1, scale2, rotX, translation;
	Ptr<Node> mul1, mul2, mul3;
};

/**
 * Build simple node graph.
 *
 * scale1 --- mul1 --- mul2 --- mul3 --- EMPTY
 *             |        |        |
 *            rotX   scale2  translation
 */
TestData prepareEnvironment()
{
	TestData ctx;

	ctx.scale1 = Builder::createOperator<EOperatorType::MakeScale>();
	ctx.rotX = Builder::createOperator<EOperatorType::MakeEulerX>();
	ctx.scale2 = Builder::createOperator<EOperatorType::MakeScale>();
	ctx.translation = Builder::createOperator<EOperatorType::MakeTranslation>();

	// Multiplicate matrices using matrix * matrix node. (Sequence is may not be
	// complete yet!)
	ctx.mul1 = Builder::createOperator<EOperatorType::MatrixMulMatrix>();
	ctx.mul2 = Builder::createOperator<EOperatorType::MatrixMulMatrix>();
	ctx.mul3 = Builder::createOperator<EOperatorType::MatrixMulMatrix>();

	plug_expectOk(ctx.scale1, ctx.mul1, 0, 0);
	plug_expectOk(ctx.rotX, ctx.mul1, 0, 1);

	plug_expectOk(ctx.mul1, ctx.mul2, 0, 0);
	plug_expectOk(ctx.scale2, ctx.mul2, 0, 1);

	plug_expectOk(ctx.mul2, ctx.mul3, 0, 0);
	plug_expectOk(ctx.translation, ctx.mul3, 0, 1);

	return ctx;
}

Ptr<Node> getRoot(Ptr<Node> node)
{
	auto parent = GraphManager::getParent(node);
	if (parent == nullptr)
	{
		return node;
	}
	return getRoot(parent);
}

TEST(NodeInterfaceTest, GetRootOwner_ShouldReturnRootNode)
{
	auto camera = GraphManager::createCamera();
	EXPECT_EQ(camera->getProj()->getRootOwner(), camera);
	EXPECT_EQ(camera->getView()->getRootOwner(), camera);
}

TEST(NodeInterfaceTest, GetParentShouldGiveValidParentNode)
{
	auto seq1 = GraphManager::createSequence();
	auto seq2 = GraphManager::createSequence();
	auto seq3 = GraphManager::createSequence();
	auto seq4 = GraphManager::createSequence();

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
	auto seq = GraphManager::createSequence();

	EXPECT_EQ(nullptr, gm::getParent(seq, 10));
	EXPECT_EQ(nullptr, gm::getParent(seq, -10));
}

TEST(NodeIntefaceTest, GetNodeInputsAndOutputs_OnComplexGraph_ReturnsValidResults)
{
	// Last node is mul3
	auto ctx = prepareEnvironment();

	auto lastNode = ctx.mul3;
	auto lastNodeInputs = GraphManager::getAllInputNodes(lastNode);
	EXPECT_EQ(2, lastNodeInputs.size());
	EXPECT_TRUE(GraphManager::getAllOutputNodes(lastNode).empty());

	auto root = getRoot(lastNode);
	auto anotherMatNode1 = Builder::createOperator<EOperatorType::MatrixAddMatrix>();
	auto anotherMatNode2 = Builder::createOperator<EOperatorType::MatrixAddMatrix>();
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

    auto* expectedOperation =
&g_transforms[static_cast<size_t>(ETransformType::Scale)];

    EXPECT_EQ(expectedOperation->first.keyWord, scale->getOperation()->keyWord);
}
 */

TEST(NodeInterfaceTest, GetAllInputNodes_ShouldReturnEmptyArrayWhenNoNodesConnected)
{
	auto seq = GraphManager::createSequence();

	auto result = GraphManager::getAllInputNodes(seq);

	EXPECT_TRUE(result.empty());
}

TEST(NodeInterfaceTest, GetAllInputNodes_ShouldReturnNodesConnectedMyInputs)
{
	auto leftSequence = GraphManager::createSequence();
	auto rightSequence = GraphManager::createSequence();
	auto matrix = GraphManager::createNode<EOperatorType::MatrixToMatrix>();

	plug_expectOk(leftSequence, rightSequence, 0, 0);
	plug_expectOk(matrix, rightSequence, 0, 1);

	auto result = GraphManager::getAllInputNodes(rightSequence);

	EXPECT_EQ(2, result.size());
}

TEST(NodeInterfaceTest, SetValueWithIndex)
{
	auto screen = Builder::createScreen();

	auto result = screen->setValue((float) 1920 / 1080, I3T_OUTPUT1);
	EXPECT_EQ(result.status, SetValueResult::Status::Ok);
}
