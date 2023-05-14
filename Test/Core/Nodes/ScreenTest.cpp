#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "Utils.h"

using namespace Core;

class ScreenTest : public GraphManagerTestFixture {};

TEST_F(ScreenTest, _)
{
	auto screen = Builder::createOperator<ENodeType::Screen>();
	auto camera = GraphManager::createCamera();

	auto t1 = Builder::createTransform<ETransformType::LookAt>()->as<TransformImpl<ETransformType::LookAt>>();
	t1->setDefaultValue("eye", generateVec3());
	t1->setDefaultValue("center", generateVec3());

	camera->getView()->addMatrix(t1);

	// Check if camera contains t1.
	EXPECT_TRUE(Math::eq(t1->getData().getMat4(), camera->getData(2).getMat4()));

	// Unplugged screen has no parent node.
	EXPECT_EQ(nullptr, GraphManager::getParent(screen));

	plug_expectOk(camera, screen, 0, 0);

	auto parent = GraphManager::getParent(screen);
	EXPECT_EQ(camera, parent->as<Camera>());

	// Get camera transform from screen node.
	EXPECT_TRUE(Math::eq(camera->getData(2).getMat4(), GraphManager::getParent(screen)->getData(2).getMat4()));
}
