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
