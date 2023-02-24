#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "../Utils.h"

using namespace Core;

struct TestTree
{
	Ptr<Sequence> rootSequence;
	Ptr<Sequence> branch1Sequence;
	Ptr<Sequence> branch2Sequence;

	std::vector<NodePtr> branch1ToRootMatrices;
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

	std::vector<Ptr<Transformation>> matrices = {
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

	std::vector<NodePtr> expectedMatrices = {
	    matrices[4], matrices[3], matrices[2], matrices[1], matrices[0],
	};

	return TestTree{root, branch1, branch2, expectedMatrices};
}

TEST(SequenceIteratorTest, MatrixIterator)
{
	auto s = arrangeSequenceTree();

	// Create sequence–root path from "branch1" sequence to root sequence.
	SequenceTree tree(s.branch1Sequence);

	std::vector<NodePtr> expectedMatrices = s.branch1ToRootMatrices;

	// Iterate through matrices.
	{
		std::vector<NodePtr> result;

		// Get iterator which points to last matrix in branch1.
		auto it = tree.begin();

		// Collect all matrices until iterator is out of tree.
		while (it != tree.end())
		{
			NodePtr matrix = *it;
			result.push_back(matrix);
			++it;
		}

		EXPECT_EQ(expectedMatrices, result);
	}
	{
		// Reverse direction.
		std::vector<NodePtr> result;

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
			NodePtr matrix = *it;
			result.push_back(matrix);
		}
		std::reverse(result.begin(), result.end());

		EXPECT_EQ(expectedMatrices, result);
	}
}
