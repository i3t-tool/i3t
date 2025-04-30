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
#include "Core/Nodes/Utils.h"

using namespace Core;

class ValueStateTest : public GraphManagerTestFixtureSuite
{};

TEST_F(ValueStateTest, _)
{
	auto leftNode = GraphManager::createNode<EOperatorType::Vector3ToVector3>();
	EXPECT_EQ(leftNode->getState(0), EValueState::Editable);

	auto rightNode = GraphManager::createNode<EOperatorType::Vector3ToVector3>();
	GraphManager::plug(leftNode, rightNode, 0, 0);
	EXPECT_EQ(rightNode->getState(0), EValueState::Locked);

	GraphManager::unplugAll(rightNode);
	EXPECT_EQ(rightNode->getState(0), EValueState::Editable);
}
