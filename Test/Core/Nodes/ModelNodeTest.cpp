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
#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "Utils.h"
#include "Viewport/entity/GameObject.h"

using namespace Core;

class ModelNodeTest : public GraphManagerTestFixture
{};

TEST_F(ModelNodeTest, ShouldConsumeTransformMatrix)
{
	// auto sequence = arrangeSequence();
	auto sequence = GraphManager::createSequence();
	auto transform = Builder::createTransform<ETransformType::Translation>();
	transform->as<Node>()->setValue(generateVec3());

	sequence->addMatrix(transform);

	auto* gameObjectVp = new Vp::GameObject();

	auto modelNode = Builder::createModelNode();

	modelNode->addUpdateCallback([&gameObjectVp, &modelNode](Node* node) {
		gameObjectVp->m_modelMatrix = modelNode->m_modelMatrix;
	});

	plug_expectOk(sequence, modelNode, 0, 0);
	{
		auto expectedMat = getMatProduct(sequence->getMatrices());
		EXPECT_EQ(expectedMat, gameObjectVp->m_modelMatrix);
	}
}
