#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

TEST(CameraNode, CameraNodeCanBePluggedToScreenNode)
{
	auto cameraNode = Builder::createNode<ENodeType::Camera>();
	auto screenNode = Builder::createNode<ENodeType::Screen>();

	auto perspectiveProj = Builder::createTransform<PerspectiveProj>();
	auto lookAt = Builder::createTransform<LookAt>();

  {
		auto result = GraphManager::plug(perspectiveProj, cameraNode, 0, 0);
	  EXPECT_EQ(ENodePlugResult::Ok, result);
	}
  {
    auto result = GraphManager::plug(lookAt, cameraNode, 0, 1);
    EXPECT_EQ(ENodePlugResult::Ok, result);
  }
  {
    auto result = GraphManager::plug(cameraNode, screenNode, 0, 0);
    EXPECT_EQ(ENodePlugResult::Ok, result);
  }

	auto expectedPV = perspectiveProj->getData().getMat4() * lookAt->getData().getMat4();
	EXPECT_EQ(expectedPV, cameraNode->getData().getMat4());
	EXPECT_EQ(expectedPV, screenNode->getData().getMat4());
}
