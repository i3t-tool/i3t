#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Utils.h"

using namespace Core;

TEST(CameraNodeTest, CameraNodeCanBePluggedToScreenNode)
{
	auto cameraNode = GraphManager::createCamera();
	auto screenNode = Builder::createNode<ENodeType::Screen>();

	auto perspectiveProj = Builder::createTransform<ETransformType::Perspective>();
	auto lookAt = Builder::createTransform<ETransformType::LookAt>();

	cameraNode->getProj()->addMatrix(perspectiveProj);
	cameraNode->getView()->addMatrix(lookAt);

	plug_expectOk(cameraNode, screenNode, 0, 0);

	auto expectedPV = perspectiveProj->getData().getMat4() * lookAt->getData().getMat4();
	EXPECT_EQ(expectedPV, cameraNode->getData(1).getMat4());
}
