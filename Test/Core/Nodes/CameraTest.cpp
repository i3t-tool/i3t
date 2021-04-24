#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Utils.h"

using namespace Core;

TEST(CameraNodeTest, CameraNodeCanBePluggedToScreenNode)
{
	auto cameraNode = Builder::createCamera();
	auto screenNode = Builder::createNode<ENodeType::Screen>();

	auto perspectiveProj = Builder::createTransform<PerspectiveProj>();
	auto lookAt = Builder::createTransform<LookAt>();

	//cameraNode->Proj()->addMatrix(perspectiveProj);
	//cameraNode->View()->addMatrix(lookAt);

	plug_expectOk(cameraNode, screenNode, 0, 0);

	auto expectedPV = perspectiveProj->getData().getMat4() * lookAt->getData().getMat4();
	EXPECT_EQ(expectedPV, cameraNode->getData(1).getMat4());
}
