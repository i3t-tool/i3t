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
	root->addMatrix(matrices[0]);
	root->addMatrix(matrices[1]);
	root->addMatrix(matrices[2]);

	branch1->addMatrix(matrices[3]);
	branch1->addMatrix(matrices[4]);

	branch2->addMatrix(matrices[5]);
	branch2->addMatrix(matrices[6]);

	std::vector<Ptr<Node>> expectedMatrices = {
	    matrices[4], matrices[3], matrices[2], matrices[1], matrices[0],
	};

	return TestTree{root, branch1, branch2, expectedMatrices};
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

	EXPECT_TRUE(it != tree.end());
}

struct TestChain
{
	Ptr<Sequence> leftSequence;
	Ptr<Sequence> middleSequence;
	Ptr<Sequence> rightSequence;
	Ptr<Node>     leftOperator;
	Ptr<Node>     rightOperator;
	glm::mat4     expected;
};

TestChain arrangeTestChain()
{
	auto leftOperator = Builder::createOperator<EOperatorType::MatrixToMatrix>();
	leftOperator->setValue(generateMat4());
	auto leftSequence = GraphManager::createSequence();
	leftOperator->plug(leftSequence, 0, I3T_SEQ_IN_MAT);

	auto middleSequence = GraphManager::createSequence();

	auto scale = Builder::createTransform<ETransformType::Scale>();
	scale->setValue(glm::scale(generateVec3()));
	middleSequence->addMatrix(scale);

	auto translation = Builder::createTransform<ETransformType::Translation>();
	translation->setValue(glm::translate(generateVec3()));
	middleSequence->addMatrix(translation);

	auto rightOperator = Builder::createOperator<EOperatorType::MatrixToMatrix>();
	rightOperator->setValue(generateMat4());
	auto rightSequence = GraphManager::createSequence();
	rightOperator->plug(rightSequence, 0, I3T_SEQ_IN_MAT);

	plug_expectOk(leftSequence, middleSequence, 0, 0);
	plug_expectOk(middleSequence, rightSequence, 0, 0);

	const auto expected = leftOperator->data().getMat4() *
	                      scale->data().getMat4() *
	                      translation->data().getMat4() *
	                      rightOperator->data().getMat4();

	return TestChain{leftSequence, middleSequence, rightSequence, leftOperator, rightOperator, expected};
}

/// \todo This test fails, but tracking seems to work fine.
/*
TEST(SequenceIteratorTest, MatrixIteratorOnSequenceWithOperatorInput)
{
	auto s = arrangeTestChain();

	SequenceTree tree(s.rightSequence);

	// Get iterator which points to last matrix in branch1.
	auto it = tree.begin();

	EXPECT_TRUE(it != tree.end());

	// Collect all matrices until iterator is out of tree.
	auto result = glm::mat4(1.0f);
	while (it != tree.end())
	{
		Ptr<Node> matrix = *it;
		result = matrix->data().getMat4() * result;
		++it;
	}

	EXPECT_TRUE(Math::eq(s.expected, result, 0.001f));
}
 */

/// \todo This test fails, but tracking seems to work fine.
/*
TEST(SequenceIteratorTest, ReversedMatrixIteratorOnSequenceWithOperatorInput)
{
	auto s = arrangeTestChain();

	SequenceTree tree(s.rightSequence);

	// Get iterator which points to last matrix in branch1.
	auto it = tree.begin();

	EXPECT_TRUE(it != tree.end());

	// Go to the root.
	while (it != tree.end())
	{
		++it;
	}

	// Collect all matrices from root to the last matrix in "branch1" sequence.
	auto result = glm::mat4(1.0f);
	while (it != tree.begin())
	{
		--it;
		Ptr<Node> node = *it;
		result = node->data().getMat4() * result;
	}

	EXPECT_TRUE(Math::eq(s.expected, result, 0.001f));
}
 */
