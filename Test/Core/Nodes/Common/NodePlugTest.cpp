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

using namespace Core;

TEST(PlugTest, DotProductOfTwoVec3NodesShouldBeCorrect)
{
	auto vec1 = Builder::createOperator<EOperatorType::Vector3ToVector3>();
	auto vec2 = Builder::createOperator<EOperatorType::Vector3ToVector3>();
	auto dotNode = Builder::createOperator<EOperatorType::Vector3DotVector3>();

	// Plug vec1 and vec2 to dotNode inputs.
	plug_expectOk(vec1, dotNode, 0, 0);
	plug_expectOk(vec2, dotNode, 0, 1);

	setValue_expectOk(vec1, glm::vec3(1.0f, 2.0f, 1.0f));
	setValue_expectOk(vec2, glm::vec3(2.0f, 1.0f, 2.0f));

	EXPECT_EQ(6.0f, dotNode->getData().getFloat());
}

TEST(PlugTest, DifferentPinTypes_NodeShouldNotBeConnected)
{
	auto fl = Builder::createOperator<EOperatorType::FloatToFloat>();
	auto vec3 = Builder::createOperator<EOperatorType::Vector3ToVector3>();

	auto result = GraphManager::plug(vec3, fl, 0, 0);

	EXPECT_EQ(ENodePlugResult::Err_MismatchedPinTypes, result);
}

TEST(PlugTest, PlugNodeOuputToTheSameNodeInput_NodeShouldNotBeConnected)
{
	auto vec3 = Builder::createOperator<EOperatorType::Vector3ToVector3>();

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
	auto f1 = Builder::createOperator<EOperatorType::FloatToFloat>();
	auto f2 = Builder::createOperator<EOperatorType::FloatToFloat>();
	auto f3 = Builder::createOperator<EOperatorType::FloatToFloat>();
	auto f4 = Builder::createOperator<EOperatorType::FloatToFloat>();

	plug_expectOk(f2, f1, 0, 0);
	plug_expectOk(f3, f2, 0, 0);
	plug_expectOk(f4, f3, 0, 0);

	auto result = GraphManager::plug(f1, f4, 0, 0);

	EXPECT_EQ(ENodePlugResult::Err_Loop, result);
}

TEST(PlugTest, UnplugMeFromPluggedNode)
{
	auto vec1 = Builder::createOperator<EOperatorType::Vector3ToVector3>();
	auto vec2 = Builder::createOperator<EOperatorType::Vector3ToVector3>();

	plug_expectOk(vec1, vec2, 0, 0);

	setValue_expectOk(vec1, glm::vec3(2.1f, 0.5f, 0.0f));

	EXPECT_FALSE(vec2->getData().getVec3().x == 0.0f);

	GraphManager::unplugAll(vec1);

	const auto& outputPins = vec1->getOutputPins();
	EXPECT_TRUE(outputPins[0].getOutComponents().empty());
}

TEST(PlugTest, UnplugInTheMiddleOfChainShouldBePossible)
{
	auto vec1 = Builder::createOperator<EOperatorType::Vector3ToVector3>();
	auto vec2 = Builder::createOperator<EOperatorType::Vector3ToVector3>();
	auto vec3 = Builder::createOperator<EOperatorType::Vector3ToVector3>();

	plug_expectOk(vec1, vec2, 0, 0);
	plug_expectOk(vec2, vec3, 0, 0);

	GraphManager::unplugAll(vec2);

	const auto& vec1OutputPins = vec1->getOutputPins();
	EXPECT_TRUE(vec1OutputPins[0].getOutComponents().empty());

	EXPECT_FALSE(vec2->getInputPins()[0].isPluggedIn());
	EXPECT_TRUE(vec2->getOutputPins()[0].getOutComponents().empty());

	EXPECT_FALSE(vec3->getInputPins()[0].isPluggedIn());
}
