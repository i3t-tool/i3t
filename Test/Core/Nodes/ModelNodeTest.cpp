#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "Utils.h"
#include "Viewport/entity/GameObject.h"
#include "World/GameObject.h"

using namespace Core;

TEST(ModelNodeTest, ShouldConsumeTransformMatrix)
{
	// auto sequence = arrangeSequence();
	auto sequence = Builder::createSequence();
	auto transform = Builder::createTransform<ETransformType::Translation>();
	transform->as<Node>()->setValue(generateVec3());

	sequence->addMatrix(transform);

	auto* gameObjectVp = new Vp::GameObject();

	auto modelNode = Builder::createModelNode();

	modelNode->addUpdateCallback([&gameObjectVp, &modelNode]()
	                             { gameObjectVp->m_modelMatrix = modelNode->m_modelMatrix; });

	plug_expectOk(sequence, modelNode, 0, 0);
	{
		auto expectedMat = getMatProduct(sequence->getMatrices());
		EXPECT_EQ(expectedMat, gameObjectVp->m_modelMatrix);
	}
}
