#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

TEST(ModelNode, ShouldConsumeTransformMatrix)
{
	auto sequence = Builder::createSequence();

  GameObject gameObject;
  auto modelNode = Builder::createNode<ENodeType::Model>();
  modelNode->setValue(static_cast<void*>(&gameObject));

	GraphManager::plug(sequence, modelNode, 0, 0);

	auto scl = Builder::createTransform<Scale>(glm::vec3(5.0f));
	auto rotZ = Builder::createTransform<EulerRotZ>(glm::radians(120.0f));

	sequence->addMatrix(scl);
	sequence->addMatrix(rotZ);

	auto expectedMat = glm::scale(glm::vec3(5.0f)) * glm::rotate(glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  auto sequenceMat = sequence->getData().getMat4();
	EXPECT_EQ(expectedMat, sequenceMat);

  auto* gameObjectPtr = static_cast<GameObject*>(modelNode->getData().getPointer());
	auto& gameObjectMat = gameObjectPtr->transformation;
	EXPECT_EQ(expectedMat, gameObjectMat);
}
