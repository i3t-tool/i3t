#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "../Utils.h"

using namespace Core;

TEST(PlugTest, DotProductOfTwoVec3NodesShouldBeCorrect)
{
	auto vec1 = Builder::createOperator<ENodeType::Vector3ToVector3>();
	auto vec2 = Builder::createOperator<ENodeType::Vector3ToVector3>();
	auto dotNode = Builder::createOperator<ENodeType::Vector3DotVector3>();

	// Plug vec1 and vec2 to dotNode inputs.
	plug_expectOk(vec1, dotNode, 0, 0);
	plug_expectOk(vec2, dotNode, 0, 1);

	setValue_expectOk(vec1, glm::vec3(1.0f, 2.0f, 1.0f));
	setValue_expectOk(vec2, glm::vec3(2.0f, 1.0f, 2.0f));

	EXPECT_EQ(6.0f, dotNode->getData().getFloat());
}

TEST(PlugTest, DifferentPinTypes_NodeShouldNotBeConnected)
{
	auto fl = Builder::createOperator<ENodeType::FloatToFloat>();
	auto vec3 = Builder::createOperator<ENodeType::Vector3ToVector3>();

	auto result = GraphManager::plug(vec3, fl, 0, 0);

	EXPECT_EQ(ENodePlugResult::Err_MismatchedPinTypes, result);
}

TEST(PlugTest, PlugNodeOuputToTheSameNodeInput_NodeShouldNotBeConnected)
{
	auto vec3 = Builder::createOperator<ENodeType::Vector3ToVector3>();

	auto result = GraphManager::plug(vec3, vec3, 0, 0);

	EXPECT_EQ(ENodePlugResult::Err_Loopback, result);
}

/**
 * F2 -> F1
 * ^     |
 * |     V
 * F3 <- F4
 */
TEST(PlugTest, PlugCannotCreateCycle)
{
	auto f1 = Builder::createOperator<ENodeType::FloatToFloat>();
	auto f2 = Builder::createOperator<ENodeType::FloatToFloat>();
	auto f3 = Builder::createOperator<ENodeType::FloatToFloat>();
	auto f4 = Builder::createOperator<ENodeType::FloatToFloat>();

	plug_expectOk(f2, f1, 0, 0);
	plug_expectOk(f3, f2, 0, 0);
	plug_expectOk(f4, f3, 0, 0);

	auto result = GraphManager::plug(f1, f4, 0, 0);

	EXPECT_EQ(ENodePlugResult::Err_Loop, result);
}

TEST(PlugTest, UnplugMeFromPluggedNode)
{
	auto vec1 = Builder::createOperator<ENodeType::Vector3ToVector3>();
	auto vec2 = Builder::createOperator<ENodeType::Vector3ToVector3>();

	plug_expectOk(vec1, vec2, 0, 0);

	setValue_expectOk(vec1, glm::vec3(2.1f, 0.5f, 0.0f));

	EXPECT_FALSE(vec2->getData().getVec3().x == 0.0f);

	GraphManager::unplugAll(vec1);

	const auto& outputPins = vec1->getOutputPins();
	EXPECT_TRUE(outputPins[0].getOutComponents().empty());
}

TEST(PlugTest, UnplugInTheMiddleOfChainShouldBePossible)
{
	auto vec1 = Builder::createOperator<ENodeType::Vector3ToVector3>();
	auto vec2 = Builder::createOperator<ENodeType::Vector3ToVector3>();
	auto vec3 = Builder::createOperator<ENodeType::Vector3ToVector3>();

	plug_expectOk(vec1, vec2, 0, 0);
	plug_expectOk(vec2, vec3, 0, 0);

	GraphManager::unplugAll(vec2);

	const auto& vec1OutputPins = vec1->getOutputPins();
	EXPECT_TRUE(vec1OutputPins[0].getOutComponents().empty());

	EXPECT_FALSE(vec2->getInputPins()[0].isPluggedIn());
	EXPECT_TRUE(vec2->getOutputPins()[0].getOutComponents().empty());

	EXPECT_FALSE(vec3->getInputPins()[0].isPluggedIn());
}
