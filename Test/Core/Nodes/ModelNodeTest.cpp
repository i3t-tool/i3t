#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "World/GameObject.h"
#include "Generator.h"
#include "Utils.h"

using namespace Core;

TEST(ModelNodeTest, ShouldConsumeTransformMatrix)
{
	// auto sequence = arrangeSequence();
	auto sequence = Builder::createSequence();
	auto transform = Builder::createTransform<Translation>(generateVec3());

	sequence->addMatrix(transform);

  /// \todo DG/MH GameObject local initialization throws an error (OpenGL context is not
  /// initialized).
  auto* gameObject = new GameObject();

  auto modelNode = Builder::createNode<ENodeType::Model>();
  setValue_expectOk(modelNode, static_cast<void*>(gameObject));

  auto* gameObjectPtr = static_cast<GameObject*>(modelNode->getData().getPointer());
  auto& gameObjectMat = gameObjectPtr->transformation;

  plug_expectOk(sequence, modelNode, 0, 0);
  {
    auto expectedMat = getMatProduct(sequence->getMatrices());
    EXPECT_EQ(expectedMat, gameObjectMat);
  }
}
