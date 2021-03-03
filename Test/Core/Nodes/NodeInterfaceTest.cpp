#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Transform.h"

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
  scale1 = Builder::createTransform<Scale>();
  rotX = Builder::createTransform<EulerRotX>(initialRot);
  scale2 = Builder::createTransform<Scale>(initialScale, Transform::g_Scale);
  translation = Builder::createTransform<Translation>(initialTransl);

	// Multiplicate matrices using matrix * matrix node. (Sequence is may not be complete yet!)
  mul1 = Builder::createNode<ENodeType::MatrixMulMatrix>();
  mul2 = Builder::createNode<ENodeType::MatrixMulMatrix>();
  mul3 = Builder::createNode<ENodeType::MatrixMulMatrix>();

  GraphManager::plug(scale1, mul1, 0, 0);
  GraphManager::plug(rotX, mul1, 0, 1);

  GraphManager::plug(mul1, mul2, 0, 0);
  GraphManager::plug(scale2, mul2, 0, 1);

  GraphManager::plug(mul2, mul3, 0, 0);
  GraphManager::plug(translation, mul3, 0, 1);

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

TEST(GetTreeRootValue, ShouldBeOk)
{
	auto lastNode = prepareEnvironment();

	// Root is "scale1", see line 18.
	auto root = getRoot(lastNode);
	EXPECT_EQ(scale1, root);

  {
    // Change scale1 (aka root) value.
    auto result = root->setValue(glm::vec3(1.0f, 5.0f, 3.0f));
    // Action should not be permitted, scale1 node is uniform scale.
    EXPECT_EQ(ValueSetResult::Status::Err_ConstraintViolation, result.status);
	}
  {
    // Change scale1 (aka root) value. Action should be ok.
    auto result = root->setValue(glm::vec3(-2.0f, -2.0f, -2.0f));
    EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
  }
	auto readOnlyMat = root->getData().getMat4();
}

TEST(GetNodeInputsAndOutputs, ReturnsValidResults)
{
	// Last node is mul3
	auto lastNode = prepareEnvironment();
	auto lastNodeInputs = GraphManager::getAllInputNodes(lastNode);
	EXPECT_EQ(2, lastNodeInputs.size());
	EXPECT_TRUE(GraphManager::getAllOutputNodes(lastNode).empty());

	auto root = getRoot(lastNode);
  auto anotherMatNode1 = Builder::createNode<ENodeType::MatrixAddMatrix>();
  auto anotherMatNode2 = Builder::createNode<ENodeType::MatrixAddMatrix>();
	GraphManager::plug(root, anotherMatNode1, 0, 0);
	GraphManager::plug(root, anotherMatNode2, 0, 0);

	auto rootOutputs = GraphManager::getAllOutputNodes(root);
	EXPECT_EQ(1 + 2, rootOutputs.size());
	EXPECT_EQ(3, GraphManager::getOutputNodes(root, 0).size());
}

/// IMPORTANT for Daniel.
/**
 *                                         /----- Sequence 2 : | Scal | Tran | (branch1)
 * Sequence 1 : | RotX | Scal | Tran | ----|
 *                                         \----- Sequence 3 : | Scal | Tran | (branch2)
 */
TEST(Sequences, MatrixIterator)
{
	// Prepare graph.
	auto root = Builder::createSequence();
	auto branch1 = Builder::createSequence();
	auto branch2 = Builder::createSequence();

	std::vector<Ptr<NodeBase>> matrices = {
      // sequence 1
      Builder::createTransform<EulerRotX>(),
      Builder::createTransform<Scale>(),
      Builder::createTransform<Translation>(),

      // sequence 2
      Builder::createTransform<Scale>(),
      Builder::createTransform<Translation>(),

			// sequence 3
      Builder::createTransform<Scale>(),
      Builder::createTransform<Translation>(),
	};

	GraphManager::plug(root, branch1, 0, 0);
	GraphManager::plug(root, branch2, 0, 0);

	// Add matrices to the sequences.
	root->addMatrix(matrices[0]);
	root->addMatrix(matrices[1]);
	root->addMatrix(matrices[2]);

	branch1->addMatrix(matrices[3]);
	branch1->addMatrix(matrices[4]);

  branch2->addMatrix(matrices[5]);
  branch2->addMatrix(matrices[6]);

	// Create sequence–root path from "branch1" sequence to root sequence.
	SequenceTree tree(branch1);

	std::vector<NodePtr> expectedMatrices = {
			matrices[4],
			matrices[3],
			matrices[2],
			matrices[1],
			matrices[0],
	};

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

TEST(TransformNodeType, TypeShouldBeDeducedFromOperationType)
{
	auto scale = Builder::createTransform<Scale>();
	EXPECT_EQ(&g_transforms[static_cast<size_t>(ETransformType::Scale)], scale->getOperation());
}
