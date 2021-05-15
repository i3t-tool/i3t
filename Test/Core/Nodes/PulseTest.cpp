#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"
#include "Utils.h"

using namespace Core;

TEST(PulseTest, PulseNodeCanTriggerCycle)
{
	auto pulse = Builder::createNode<ENodeType::Pulse>();
	auto cycle = GraphManager::createCycle();

  plug_expectOk(pulse, cycle, 0, CycleInternals::in_play);
	EXPECT_TRUE(!cycle->isRunning());

	pulse->spreadSignal();
	EXPECT_TRUE(cycle->isRunning());

	GraphManager::unplugAll(pulse);

	plug_expectOk(pulse, cycle, 0, CycleInternals::in_pause);

  pulse->spreadSignal();
  EXPECT_FALSE(cycle->isRunning());
}
