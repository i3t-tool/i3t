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

	EXPECT_TRUE(Math::eq(
			cameraNode->getProj()->getData(I3T_SEQ_OUT_MAT).getMat4(),
			perspectiveProj->getData().getMat4()
	));
	EXPECT_TRUE(Math::eq(
			cameraNode->getView()->getData(I3T_SEQ_OUT_MAT).getMat4(),
			lookAt->getData().getMat4()
	));

	plug_expectOk(cameraNode, screenNode, 0, 0);

	auto expectedPV = perspectiveProj->getData().getMat4() * lookAt->getData().getMat4();
	EXPECT_EQ(expectedPV, cameraNode->getData(1).getMat4());
}

TEST(CameraNodeTest, CameraAndSequenceCannotBeConnected)
{
	auto camera   = GraphManager::createCamera();
	auto sequence = Builder::createSequence();

	EXPECT_TRUE(camera->getOutPin(I3T_CAMERA_OUT_MUL).isDisabled());

	const auto result = GraphManager::plug(camera, sequence, I3T_CAMERA_OUT_MUL, I3T_SEQ_IN_MUL);
	EXPECT_EQ(result, ENodePlugResult::Err_DisabledPin);
}
