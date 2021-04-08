#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "Utils.h"

using namespace Core;

TEST(ModelNodeTest, ShouldConsumeTransformMatrix)
{
	auto sequence = arrangeSequence();

	GameObject gameObject;
	auto modelNode = Builder::createNode<ENodeType::Model>();
	setValue_expectOk(modelNode, static_cast<void*>(&gameObject));

	auto* gameObjectPtr = static_cast<GameObject*>(modelNode->getData().getPointer());
	auto& gameObjectMat = gameObjectPtr->transformation;

	plug_expectOk(sequence, modelNode, 0, 0);
	{
		auto expectedMat = getMatProduct(sequence->getMatrices());
		EXPECT_EQ(expectedMat, gameObjectMat);
	}
}
