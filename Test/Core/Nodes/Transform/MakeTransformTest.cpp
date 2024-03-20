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

#include "../Utils.h"
#include "Generator.h"
#include "NodeGenerator.h"

using namespace Core;

TEST(MakeTransformTest, MakeTranslationNodeShouldBeValid)
{
	auto vec3 = Core::Builder::createOperator<EOperatorType::Vector3ToVector3>();
	auto initialTranslation = generateVec3();

	auto makeTranslation = Core::Builder::createOperator<EOperatorType::MakeTranslation>();

	plug_expectOk(vec3, makeTranslation, 0, 0);

	setValue_expectOk(vec3, initialTranslation);

	auto expectedNodeValue = glm::translate(initialTranslation);
	EXPECT_EQ(expectedNodeValue, makeTranslation->data().getMat4());
}

TEST(MakeTransformTest, MakeEulerRotsNodeShouldBeValid)
{
	auto floatNode = Core::Builder::createOperator<EOperatorType::FloatToFloat>();
	auto rotRads = generateFloat();

	auto makeRotX = Core::Builder::createOperator<EOperatorType::MakeEulerX>();
	auto makeRotY = Core::Builder::createOperator<EOperatorType::MakeEulerY>();
	auto makeRotZ = Core::Builder::createOperator<EOperatorType::MakeEulerZ>();

	plug_expectOk(floatNode, makeRotX, 0, 0);
	plug_expectOk(floatNode, makeRotY, 0, 0);
	plug_expectOk(floatNode, makeRotZ, 0, 0);

	setValue_expectOk(floatNode, rotRads);

	auto expectedRotXMat = glm::rotate(rotRads, glm::vec3(1.0f, 0.0f, 0.0f));
	auto expectedRotYMat = glm::rotate(rotRads, glm::vec3(0.0f, 1.0f, 0.0f));
	auto expectedRotZMat = glm::rotate(rotRads, glm::vec3(0.0f, 0.0f, 1.0f));
	EXPECT_EQ(expectedRotXMat, makeRotX->data().getMat4());
	EXPECT_EQ(expectedRotYMat, makeRotY->data().getMat4());
	EXPECT_EQ(expectedRotZMat, makeRotZ->data().getMat4());
}

TEST(MakeTransformTest, MakeAxisRotShouldBeValid)
{
	auto floatNode = Core::Builder::createOperator<EOperatorType::FloatToFloat>();
	auto axisNode = Core::Builder::createOperator<EOperatorType::Vector3ToVector3>();
	float rotRads = generateFloat();
	auto axis = generateVec3();

	auto makeAxisRotNode = Core::Builder::createOperator<EOperatorType::MakeAxisAngle>();

	plug_expectOk(floatNode, makeAxisRotNode, 0, 0);
	plug_expectOk(axisNode, makeAxisRotNode, 0, 1);

	setValue_expectOk(floatNode, rotRads);
	setValue_expectOk(axisNode, axis);

	auto expectedNodeValue = glm::rotate(rotRads, axis);
	EXPECT_EQ(expectedNodeValue, makeAxisRotNode->data().getMat4());
}

TEST(MakeTransformTest, MakeOrthoShouldBeValid)
{
	auto makeOrthoNode = Core::Builder::createOperator<EOperatorType::MakeOrtho>();

	auto [inputs, inputNodes] = generateFloatInputs<6>(makeOrthoNode);

	auto expectedOrtho = glm::ortho(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);

	EXPECT_EQ(expectedOrtho, makeOrthoNode->data().getMat4());
}

TEST(MakeTransformTest, MakePerspectiveShouldBeValid)
{
	auto makePerspectiveNode = Core::Builder::createOperator<EOperatorType::MakePerspective>();

	auto [inputValues, inputNodes] = generateFloatInputs<4>(makePerspectiveNode);

	auto expectedMat = glm::perspective(inputValues[0], inputValues[1], inputValues[2], inputValues[3]);

	EXPECT_EQ(expectedMat, makePerspectiveNode->data().getMat4());
}

TEST(MakeTransformTest, MakeFrustumShouldBeValid)
{
	auto makeFrustumNode = Core::Builder::createOperator<EOperatorType::MakeFrustum>();

	auto [inputs, inputNodes] = generateFloatInputs<6>(makeFrustumNode);

	auto expectedOrtho = glm::frustum(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);

	EXPECT_EQ(expectedOrtho, makeFrustumNode->data().getMat4());
}

TEST(MakeTransformTest, MakeLookAtShouldBeValid)
{
	auto makeLookAtNode = Builder::createOperator<EOperatorType::MakeLookAt>();

	auto vec1 = Builder::createOperator<EOperatorType::Vector3ToVector3>();
	auto vec2 = Builder::createOperator<EOperatorType::Vector3ToVector3>();
	auto vec3 = Builder::createOperator<EOperatorType::Vector3ToVector3>();

	plug_expectOk(vec1, makeLookAtNode, 0, 0);
	plug_expectOk(vec2, makeLookAtNode, 0, 1);
	plug_expectOk(vec3, makeLookAtNode, 0, 2);

	std::array values = {generateVec3(), generateVec3(), generateVec3()};

	setValue_expectOk(vec1, values[0]);
	setValue_expectOk(vec2, values[1]);
	setValue_expectOk(vec3, values[2]);

	auto expectedMat = glm::lookAt(values[0], values[1], values[2]);

	EXPECT_EQ(expectedMat, makeLookAtNode->data().getMat4());
}
