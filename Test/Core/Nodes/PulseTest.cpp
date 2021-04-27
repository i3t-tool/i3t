#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"
#include "Utils.h"

using namespace Core;

TEST(PulseTest, PulseNodeCanTriggerCycle)
{
	auto pulse = Builder::createNode<ENodeType::Pulse>();
	auto cycle = GraphManager::createCycle();

  plug_expectOk(pulse, cycle, 0, CycleInternals::in_play);

	pulse->updateValues();
	EXPECT_TRUE(cycle->isRunning());

	GraphManager::unplugAll(pulse);

	plug_expectOk(pulse, cycle, 0, CycleInternals::in_pause);

  pulse->updateValues();
  EXPECT_FALSE(cycle->isRunning());
}
