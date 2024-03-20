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

using namespace Core;

/**
 * Float node should contain <vec3|vec3> value.
 *
 * vec3 --- dot --- float
 *           |
 *          vec3
 */
TEST(SpreadSignalTest, ValuesShouldBeSpreadThroughConnectedNodes)
{
	auto vec1Node = Core::Builder::createOperator<EOperatorType::Vector3ToVector3>();
	auto vec2Node = Core::Builder::createOperator<EOperatorType::Vector3ToVector3>();
	auto dotNode = Core::Builder::createOperator<EOperatorType::Vector3DotVector3>();
	auto floatNode = Core::Builder::createOperator<EOperatorType::FloatToFloat>();

	// Plug vec1 and vec2 to dotNode inputs.
	plug_expectOk(vec1Node, dotNode, 0, 0);
	plug_expectOk(vec2Node, dotNode, 0, 1);
	plug_expectOk(dotNode, floatNode, 0, 0);

	auto vec1 = generateVec3();
	auto vec2 = generateVec3();

	setValue_expectOk(vec1Node, vec1);
	setValue_expectOk(vec2Node, vec2);

	float expectedValue = glm::dot(vec1, vec2);
	EXPECT_EQ(expectedValue, floatNode->data().getFloat());
}
