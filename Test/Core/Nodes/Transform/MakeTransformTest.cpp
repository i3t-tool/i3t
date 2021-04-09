#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "../Utils.h"
#include "Generator.h"
#include "NodeGenerator.h"

using namespace Core;

TEST(NodesTest, MakeTranslationNodeShouldBeValid)
{
	auto vec3 = Core::Builder::createNode<ENodeType::Vector3>();
	auto initialTranslation = generateVec3();

	auto makeTranslation = Core::Builder::createNode<ENodeType::MakeTranslation>();

	plug_expectOk(vec3, makeTranslation, 0, 0);

	setValue_expectOk(vec3, initialTranslation);

	auto expectedNodeValue = glm::translate(initialTranslation);
	EXPECT_EQ(expectedNodeValue, makeTranslation->getData().getMat4());
}

TEST(NodesTest, MakeEulerRotsNodeShouldBeValid)
{
	auto floatNode = Core::Builder::createNode<ENodeType::Float>();
	auto rotRads = generateFloat();

	auto makeRotX = Core::Builder::createNode<ENodeType::MakeEulerX>();
	auto makeRotY = Core::Builder::createNode<ENodeType::MakeEulerY>();
	auto makeRotZ = Core::Builder::createNode<ENodeType::MakeEulerZ>();

	plug_expectOk(floatNode, makeRotX, 0, 0);
	plug_expectOk(floatNode, makeRotY, 0, 0);
	plug_expectOk(floatNode, makeRotZ, 0, 0);

	setValue_expectOk(floatNode, rotRads);

	auto expectedRotXMat = glm::rotate(rotRads, glm::vec3(1.0f, 0.0f, 0.0f));
	auto expectedRotYMat = glm::rotate(rotRads, glm::vec3(0.0f, 1.0f, 0.0f));
	auto expectedRotZMat = glm::rotate(rotRads, glm::vec3(0.0f, 0.0f, 1.0f));
	EXPECT_EQ(expectedRotXMat, makeRotX->getData().getMat4());
	EXPECT_EQ(expectedRotYMat, makeRotY->getData().getMat4());
	EXPECT_EQ(expectedRotZMat, makeRotZ->getData().getMat4());
}

TEST(NodesTest, MakeAxisRotShouldBeValid)
{
	auto floatNode = Core::Builder::createNode<ENodeType::Float>();
	auto axisNode = Core::Builder::createNode<ENodeType::Vector3>();
	float rotRads = generateFloat();
	auto axis = generateVec3();

	auto makeAxisRotNode = Core::Builder::createNode<ENodeType::MakeAxisAngle>();

	plug_expectOk(floatNode, makeAxisRotNode, 0, 0);
	plug_expectOk(axisNode, makeAxisRotNode, 0, 1);

	setValue_expectOk(floatNode, rotRads);
	setValue_expectOk(axisNode, axis);

	auto expectedNodeValue = glm::rotate(rotRads, axis);
	EXPECT_EQ(expectedNodeValue, makeAxisRotNode->getData().getMat4());
}

TEST(NodesTest, MakeOrthoShouldBeValid)
{
	auto makeOrthoNode = Core::Builder::createNode<ENodeType::MakeOrtho>();

	auto [inputs, inputNodes] = generateFloatInputs<6>(makeOrthoNode);

	auto expectedOrtho = glm::ortho(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);

	EXPECT_EQ(expectedOrtho, makeOrthoNode->getData().getMat4());
}

TEST(NodesTest, MakePerspectiveShouldBeValid)
{
	auto makePerspectiveNode = Core::Builder::createNode<ENodeType::MakePerspective>();

	auto [inputValues, inputNodes] = generateFloatInputs<4>(makePerspectiveNode);

	auto expectedMat = glm::perspective(inputValues[0], inputValues[1], inputValues[2], inputValues[3]);

	EXPECT_EQ(expectedMat, makePerspectiveNode->getData().getMat4());
}

TEST(NodesTest, MakeFrustumShouldBeValid)
{
	auto makeFrustumNode = Core::Builder::createNode<ENodeType::MakeFrustum>();

	auto [inputs, inputNodes] = generateFloatInputs<6>(makeFrustumNode);

	auto expectedOrtho = glm::frustum(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);

	EXPECT_EQ(expectedOrtho, makeFrustumNode->getData().getMat4());
}

TEST(NodesTest, MakeLookAtShouldBeValid)
{
	auto makeLookAtNode = Builder::createNode<ENodeType::MakeLookAt>();

	auto vec1 = Builder::createNode<ENodeType::Vector3>();
	auto vec2 = Builder::createNode<ENodeType::Vector3>();
	auto vec3 = Builder::createNode<ENodeType::Vector3>();

	plug_expectOk(vec1, makeLookAtNode, 0, 0);
	plug_expectOk(vec2, makeLookAtNode, 0, 1);
	plug_expectOk(vec3, makeLookAtNode, 0, 2);

	std::array values = {generateVec3(), generateVec3(), generateVec3()};

	setValue_expectOk(vec1, values[0]);
	setValue_expectOk(vec2, values[1]);
	setValue_expectOk(vec3, values[2]);

	auto expectedMat = glm::lookAt(values[0], values[1], values[2]);

	EXPECT_EQ(expectedMat, makeLookAtNode->getData().getMat4());
}
