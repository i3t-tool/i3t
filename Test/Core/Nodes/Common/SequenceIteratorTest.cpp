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

#include "../Utils.h"
#include "Generator.h"

using namespace Core;

struct TestTree
{
	Ptr<Sequence> rootSequence;
	Ptr<Sequence> branch1Sequence;
	Ptr<Sequence> branch2Sequence;

	std::vector<Ptr<Node>> branch1ToRootMatrices;
};

/**
 *                                         /----- Sequence 2 : | Scal | Tran |
 * (branch1) Sequence 1 : | RotX | Scal | Tran | ----|
 *                                         \----- Sequence 3 : | Scal | Tran |
 * (branch2)
 */
TestTree arrangeSequenceTree()
{
	// Prepare graph.
	auto root = GraphManager::createSequence();
	auto branch1 = GraphManager::createSequence();
	auto branch2 = GraphManager::createSequence();

	std::vector<Ptr<Transform>> matrices = {
	    // sequence 1
	    Builder::createTransform<ETransformType::EulerX>(),
	    Builder::createTransform<ETransformType::Scale>(),
	    Builder::createTransform<ETransformType::Translation>(),

	    // sequence 2
	    Builder::createTransform<ETransformType::Scale>(),
	    Builder::createTransform<ETransformType::Translation>(),

	    // sequence 3
	    Builder::createTransform<ETransformType::Scale>(),
	    Builder::createTransform<ETransformType::Translation>(),
	};

	plug_expectOk(root, branch1, 0, 0);
	plug_expectOk(root, branch2, 0, 0);

	// Add matrices to the sequences.
	root->pushMatrix(matrices[0]);
	root->pushMatrix(matrices[1]);
	root->pushMatrix(matrices[2]);

	branch1->pushMatrix(matrices[3]);
	branch1->pushMatrix(matrices[4]);

	branch2->pushMatrix(matrices[5]);
	branch2->pushMatrix(matrices[6]);

	std::vector<Ptr<Node>> expectedMatrices = {
	    matrices[4], matrices[3], matrices[2], matrices[1], matrices[0],
	};

	return TestTree{root, branch1, branch2, expectedMatrices};
}

TEST(SequenceIteratorTest, BeginAndEnd)
{
	auto s = arrangeSequenceTree();

	// Create sequence–root path from "branch1" sequence to root sequence.
	SequenceTree tree(s.branch1Sequence);

	// Get iterator which points to last matrix in branch1.
	auto it = tree.begin();
	EXPECT_TRUE(it != tree.end());
	EXPECT_TRUE(*it == s.branch1Sequence->getMatrices().back());
}

/// X - translation on X axis
/// Y - translation on Y axis
/// Z - translation on Z axis
///
/// ```
///                      /- matrix1 (Z)                                     /- matrix2 (Z)
///                     /                                                  /
/// | sequence 1 | - | sequence 2 | - | sequence 3 | - | sequence 4 | - | sequence 5 |
/// | Y, X       |   | X          |   | X          |   |            |   | X          |
/// ```
struct TrickyTestTree
{
	Ptr<Sequence> sequence1;
	Ptr<Sequence> sequence2;
	Ptr<Sequence> sequence3;
	Ptr<Sequence> sequence4;
	Ptr<Sequence> sequence5;

	Ptr<Node> matrix1;
	Ptr<Node> matrix2;

	glm::vec3 translationX = {1, 0, 0};
	glm::vec3 translationY = {0, 1, 0};
	glm::vec3 translationZ = {0, 0, 1};

	TrickyTestTree()
	{
		sequence1 = GraphManager::createSequence();
		sequence1->pushMatrix(createTranslation(translationY));
		sequence1->pushMatrix(createTranslation(translationX));
		sequence2 = GraphManager::createSequence();
		sequence2->pushMatrix(createTranslation(translationX));
		sequence3 = GraphManager::createSequence();
		sequence3->pushMatrix(createTranslation(translationX));
		sequence4 = GraphManager::createSequence();
		sequence5 = GraphManager::createSequence();
		sequence5->pushMatrix(createTranslation(translationX));

		matrix1 = GraphManager::createNode<EOperatorType::MakeTranslation>();
		matrix1->setValue(glm::translate(translationZ));
		matrix2 = GraphManager::createNode<EOperatorType::MakeTranslation>();
		matrix2->setValue(glm::translate(translationZ));

		plug_expectOk(sequence1, sequence2, 0, 0);

		plug_expectOk(sequence2, sequence3, 0, 0);
		plug_expectOk(matrix1, sequence2, 0, I3T_SEQ_IN_MAT);

		plug_expectOk(sequence3, sequence4, 0, 0);

		plug_expectOk(sequence4, sequence5, 0, 0);

		plug_expectOk(matrix2, sequence5, 0, I3T_SEQ_IN_MAT);
	}

private:
	static Ptr<Transform> createTranslation(glm::vec3 translation)
	{
		auto transform = Builder::createTransform<ETransformType::Translation>();
		transform->setDefaultValue("translate", translation);

		return transform;
	}
};

TEST(SequenceIteratorTest, Advance)
{
	TrickyTestTree s;

	SequenceTree tree(s.sequence5);

	auto it = tree.begin();
	ASSERT_TRUE(it != tree.end());

	Ptr<Node> matrix;

	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence5.get());
	EXPECT_TRUE(it.transformInfo().isExternal);
	EXPECT_EQ(matrix, s.matrix2);
	++it;
	ASSERT_TRUE(it != tree.end());

	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence3.get());
	EXPECT_FALSE(it.transformInfo().isExternal);
	EXPECT_EQ(matrix, s.sequence3->getMatrices().back());
	++it;
	ASSERT_TRUE(it != tree.end());

	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence2.get());
	EXPECT_TRUE(it.transformInfo().isExternal);
	EXPECT_EQ(matrix, s.matrix1);
	++it;
	ASSERT_TRUE(it != tree.end());

	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence1.get());
	EXPECT_FALSE(it.transformInfo().isExternal);
	EXPECT_EQ(matrix, s.sequence1->getMatrices()[1]);
	++it;
	ASSERT_TRUE(it != tree.end());

	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence1.get());
	EXPECT_FALSE(it.transformInfo().isExternal);
	EXPECT_EQ(matrix, s.sequence1->getMatrices()[0]);
	++it;

	// Iterator is invalid.
	EXPECT_EQ(it.getSequence(), s.sequence1.get());
	EXPECT_TRUE(it == tree.end());
}

TEST(SequenceIteratorTest, Withdraw)
{
	TrickyTestTree s;

	SequenceTree tree(s.sequence5);

	auto it = tree.end();
	ASSERT_TRUE(it != tree.begin());
	ASSERT_TRUE(it.getSequence() == s.sequence1.get());

	--it;
	auto matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence1.get());
	EXPECT_EQ(matrix, s.sequence1->getMatrices()[0]);

	--it;
	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence1.get());
	EXPECT_EQ(matrix, s.sequence1->getMatrices()[1]);

	--it;
	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence2.get());
	EXPECT_EQ(matrix, s.matrix1);

	--it;
	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence3.get());
	EXPECT_EQ(matrix, s.sequence3->getMatrices().back());

	--it;
	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence5.get());
	EXPECT_EQ(matrix, s.matrix2);

	EXPECT_TRUE(it == tree.begin());
}

TEST(SequenceIteratorTest, collect)
{
	TrickyTestTree s;
	SequenceTree tree(s.sequence5);

	std::vector<Ptr<Node>> expected = {
	    s.matrix2,
	    s.sequence3->getMatrices().back(),
	    s.matrix1,
	    s.sequence1->getMatrices()[1],
	    s.sequence1->getMatrices()[0],
	};

	const auto result = tree.begin().collect();

	EXPECT_EQ(expected, result);
}

TEST(SequenceIteratorTest, MatrixIterator)
{
	auto s = arrangeSequenceTree();

	// Create sequence–root path from "branch1" sequence to root sequence.
	SequenceTree tree(s.branch1Sequence);

	std::vector<Ptr<Node>> expectedMatrices = s.branch1ToRootMatrices;

	// Iterate through matrices.
	{
		std::vector<Ptr<Node>> result;

		// Get iterator which points to last matrix in branch1.
		auto it = tree.begin();

		EXPECT_TRUE(it != tree.end());

		// Collect all matrices until iterator is out of tree.
		while (it != tree.end())
		{
			Ptr<Node> matrix = *it;
			result.push_back(matrix);
			++it;
		}

		EXPECT_EQ(expectedMatrices, result);
	}
	{
		// Reverse direction.
		std::vector<Ptr<Node>> result;

		// Get iterator which points to last matrix in branch1.
		auto it = tree.begin();

		// Go to the root.
		while (it != tree.end())
		{
			++it;
		}

		// Collect all matrices from root to the last matrix in "branch1" sequence.
		while (it != tree.begin())
		{
			--it;
			Ptr<Node> matrix = *it;
			result.push_back(matrix);
		}
		std::reverse(result.begin(), result.end());

		EXPECT_EQ(expectedMatrices, result);
	}
}

TEST(SequenceIteratorTest, MatrixIteratorOnTwoEmptySequences)
{
	auto leftSequence = GraphManager::createSequence();
	auto rightSequence = GraphManager::createSequence();

	plug_expectOk(leftSequence, rightSequence, 0, 0);

	SequenceTree tree(rightSequence);

	// Get iterator which points to last matrix in branch1.
	auto it = tree.begin();

	EXPECT_TRUE(it == tree.end());
}

TEST(SequenceIteratorTest, GetNonemptyParentSequence)
{
	auto s1 = GraphManager::createSequence();
	auto s2 = GraphManager::createSequence();
	auto s3 = GraphManager::createSequence();

	plug_expectOk(s1, s2, I3T_SEQ_OUT_MAT, I3T_SEQ_IN_MAT);
	plug_expectOk(s2, s3, I3T_SEQ_OUT_MUL, I3T_SEQ_IN_MUL);

	auto t1 = Builder::createTransform<ETransformType::Translation>();
	t1->setDefaultValue("translate", glm::vec3(1, 1, 1));
	s1->pushMatrix(t1);
	/*
	s1->pushMatrix(Builder::createTransform<ETransformType::Translation>());
	s1->pushMatrix(Builder::createTransform<ETransformType::Translation>());
	s1->pushMatrix(Builder::createTransform<ETransformType::Translation>());
	s1->pushMatrix(Builder::createTransform<ETransformType::Translation>());
	 */

	s2->pushMatrix(Builder::createTransform<ETransformType::Translation>());
	// Never true.
	// EXPECT_EQ(s2->getInternalData().getMat4(), glm::translate(glm::vec3(1, 1, 1)));

	s3->pushMatrix(Builder::createTransform<ETransformType::Translation>());

	auto s1Parent = getNonemptyParentSequence(s1);
	auto s2Parent = getNonemptyParentSequence(s2);
	auto s3Parent = getNonemptyParentSequence(s3);
	EXPECT_EQ(s1Parent, nullptr);
	EXPECT_EQ(s2Parent, nullptr);
	EXPECT_EQ(s3Parent, s2);

	SequenceTree tree(s3);
	auto [matrices, info] = tree.begin().collectWithInfo();
	EXPECT_EQ(matrices.size(), 2);
	EXPECT_EQ(info.size(), 2);

	EXPECT_EQ(info[0].isExternal, false);
	EXPECT_EQ(info[0].sequence, s3.get());

	EXPECT_EQ(info[1].isExternal, true);
	// EXPECT_NE(info[1].sequence, s1.get());
	// EXPECT_NE(info[1].sequence, s3.get());
	EXPECT_EQ(info[1].sequence, s2.get());
	EXPECT_EQ(info[1].sequence->getId(), s2->getId());

	auto* t = s3->startTracking(TrackingDirection::LeftToRight, std::vector<UPtr<IModelProxy>>());
	t->setParam(0.2f);
	EXPECT_NE(t->result().interpolatedTransformID, s1->getId());
	EXPECT_EQ(t->result().interpolatedTransformID, s2->getId());
	EXPECT_NE(t->result().interpolatedTransformID, s3->getId());
}

TEST(SequenceIteratorTest, MatrixInputPlugged)
{
	const auto sequence = GraphManager::createSequence();
	const auto matrix = Builder::createOperator<Core::EOperatorType::MatrixToMatrix>();
	plug_expectOk(matrix, sequence, 0, I3T_SEQ_IN_MAT);

	SequenceTree tree(sequence);

	{
		auto it = tree.begin();
		EXPECT_TRUE(it != tree.end());
		EXPECT_EQ(*it, matrix);
	}
}
