#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"
#include "Utils.h"

using namespace Core;

TEST_F(CycleTestFixture, PulseNodeCanTriggerCycle)
{
	auto pulse = Builder::createOperator<EOperatorType::Pulse>();
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
