/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Core/Nodes/Screen.h"


#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "Utils.h"

using namespace Core;

class ScreenTest : public GraphManagerTestFixtureSuite
{};

TEST_F(ScreenTest, _)
{
	auto screen = Builder::createScreen();
	auto camera = GraphManager::createCamera();

	auto t1 = Builder::createTransform<ETransformType::LookAt>()->as<TransformImpl<ETransformType::LookAt>>();
	t1->setDefaultValue("eye", generateVec3());
	t1->setDefaultValue("center", generateVec3());

	camera->getView()->pushMatrix(t1);

	// Check if camera contains t1.
	EXPECT_TRUE(Math::eq(t1->data().getMat4(), camera->data(I3T_CAMERA_OUT_MATRIX).getMat4()));

	// Unplugged screen has no parent node.
	EXPECT_EQ(nullptr, GraphManager::getParent(screen));

	plug_expectOk(camera, screen, 0, 0);

	auto parent = GraphManager::getParent(screen);
	EXPECT_EQ(camera, parent->as<Camera>());

	// Get camera transform from screen node.
	EXPECT_TRUE(Math::eq(camera->data(2).getMat4(), GraphManager::getParent(screen)->data(2).getMat4()));
}
