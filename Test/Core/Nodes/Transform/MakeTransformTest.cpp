#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Core/Nodes/Transform/NodeGenerator.h"
#include "Generator.h"

using namespace Core;

TEST(Nodes, MakeTranslationNodeShouldBeValid)
{
	auto vec3 = Builder::createNode<ENodeType::Vector3>();
	auto initialTranslation = generateVec3();

	auto makeTranslation = Builder::createNode<ENodeType::MakeTranslation>();

	GraphManager::plug(vec3, makeTranslation);

	auto setValueResult = vec3->setValue(initialTranslation);
	EXPECT_EQ(ValueSetResult::Status::Ok, setValueResult.status);

	auto expectedNodeValue = glm::translate(initialTranslation);
	EXPECT_EQ(expectedNodeValue, makeTranslation->getData().getMat4());
}

TEST(Nodes, MakeEulerRotsNodeShouldBeValid)
{
	auto floatNode = Builder::createNode<ENodeType::Float>();
	auto rotRads = generateFloat();

	auto makeRotX = Builder::createNode<ENodeType::MakeEulerX>();
	auto makeRotY = Builder::createNode<ENodeType::MakeEulerY>();
	auto makeRotZ = Builder::createNode<ENodeType::MakeEulerZ>();

	GraphManager::plug(floatNode, makeRotX);
	GraphManager::plug(floatNode, makeRotY);
	GraphManager::plug(floatNode, makeRotZ);

	auto setValueResult = floatNode->setValue(rotRads);
	EXPECT_EQ(ValueSetResult::Status::Ok, setValueResult.status);

	auto expectedRotXMat = glm::rotate(rotRads, glm::vec3(1.0f, 0.0f, 0.0f));
	auto expectedRotYMat = glm::rotate(rotRads, glm::vec3(0.0f, 1.0f, 0.0f));
	auto expectedRotZMat = glm::rotate(rotRads, glm::vec3(0.0f, 0.0f, 1.0f));
	EXPECT_EQ(expectedRotXMat, makeRotX->getData().getMat4());
	EXPECT_EQ(expectedRotYMat, makeRotY->getData().getMat4());
	EXPECT_EQ(expectedRotZMat, makeRotZ->getData().getMat4());
}

TEST(Nodes, MakeAxisRotShouldBeValid)
{
	auto floatNode = Builder::createNode<ENodeType::Float>();
	auto axisNode = Builder::createNode<ENodeType::Vector3>();
	float rotRads = generateFloat();
	auto axis = generateVec3();

	auto makeAxisRotNode = Builder::createNode<ENodeType::MakeAxisAngle>();

	GraphManager::plug(floatNode, makeAxisRotNode, 0, 0);
	GraphManager::plug(axisNode, makeAxisRotNode, 0, 1);

	{
		auto setValueResult = floatNode->setValue(rotRads);
		EXPECT_EQ(ValueSetResult::Status::Ok, setValueResult.status);
	}
	{
		auto setValueResult = axisNode->setValue(axis);
		EXPECT_EQ(ValueSetResult::Status::Ok, setValueResult.status);
	}

	auto expectedNodeValue = glm::rotate(rotRads, axis);
	EXPECT_EQ(expectedNodeValue, makeAxisRotNode->getData().getMat4());
}

TEST(Nodes, MakeOrthoShouldBeValid)
{
	auto makeOrthoNode = Builder::createNode<ENodeType::MakeOrtho>();

	auto [inputs, inputNodes] = generateFloatInputs<6>(makeOrthoNode);

	auto expectedOrtho = glm::ortho(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);

	EXPECT_EQ(expectedOrtho, makeOrthoNode->getData().getMat4());
}

TEST(Nodes, MakePerspectiveShouldBeValid)
{
	auto makePerspectiveNode = Builder::createNode<ENodeType::MakePerspective>();

	auto [inputValues, inputNodes] = generateFloatInputs<4>(makePerspectiveNode);

	auto expectedMat = glm::perspective(inputValues[0], inputValues[1], inputValues[2], inputValues[3]);

	EXPECT_EQ(expectedMat, makePerspectiveNode->getData().getMat4());
}

TEST(Nodes, MakeFrustumShouldBeValid)
{
	auto makeFrustumNode = Builder::createNode<ENodeType::MakeFrustum>();

	auto [inputs, inputNodes] = generateFloatInputs<6>(makeFrustumNode);

	auto expectedOrtho = glm::frustum(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);

	EXPECT_EQ(expectedOrtho, makeFrustumNode->getData().getMat4());
}

TEST(Nodes, MakeLookAtShouldBeValid)
{
	auto makeLookAtNode = Builder::createNode<ENodeType::MakeLookAt>();

	auto vec1 = Builder::createNode<ENodeType::Vector3>();
	auto vec2 = Builder::createNode<ENodeType::Vector3>();
	auto vec3 = Builder::createNode<ENodeType::Vector3>();

	GraphManager::plug(vec1, makeLookAtNode, 0, 0);
	GraphManager::plug(vec2, makeLookAtNode, 0, 1);
	GraphManager::plug(vec3, makeLookAtNode, 0, 2);

	std::array values = {generateVec3(), generateVec3(), generateVec3()};

	vec1->setValue(values[0]);
	vec2->setValue(values[1]);
	vec3->setValue(values[2]);

	auto expectedMat = glm::lookAt(values[0], values[1], values[2]);

	EXPECT_EQ(expectedMat, makeLookAtNode->getData().getMat4());
}
