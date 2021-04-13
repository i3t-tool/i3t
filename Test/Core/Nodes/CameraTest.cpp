#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Utils.h"

using namespace Core;

TEST(CameraNodeTest, CameraNodeCanBePluggedToScreenNode)
{
	auto cameraNode = Builder::createNode<ENodeType::Camera>();
	auto screenNode = Builder::createNode<ENodeType::Screen>();

	auto perspectiveProj = Builder::createTransform<PerspectiveProj>();
	auto lookAt = Builder::createTransform<LookAt>();

	plug_expectOk(perspectiveProj, cameraNode, 0, 0);
	plug_expectOk(lookAt, cameraNode, 0, 1);
	plug_expectOk(cameraNode, screenNode, 0, 0);

	auto expectedPV = perspectiveProj->getData().getMat4() * lookAt->getData().getMat4();
	EXPECT_EQ(expectedPV, cameraNode->getData().getMat4());
	EXPECT_EQ(expectedPV, screenNode->getData().getMat4());
}
