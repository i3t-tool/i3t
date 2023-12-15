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
#include "Utils.h"

using namespace Core;

TEST_F(CycleTestFixture, PulseNodeCanTriggerCycle)
{
	auto pulse = Builder::createOperator<EOperatorType::PulseToPulse>();
	auto cycle = GraphManager::createCycle();

	plug_expectOk(pulse, cycle, I3T_OUTPUT0, I3T_CYCLE_IN_PLAY);
	EXPECT_TRUE(!cycle->isRunning());

	pulse->pulse(I3T_OUTPUT0);
	EXPECT_TRUE(cycle->isRunning());

	GraphManager::unplugAll(pulse);

	plug_expectOk(pulse, cycle, I3T_OUTPUT0, I3T_CYCLE_IN_PAUSE);

	pulse->pulse(I3T_OUTPUT0);
	EXPECT_FALSE(cycle->isRunning());
}
