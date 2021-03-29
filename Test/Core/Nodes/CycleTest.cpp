#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

double sPerFrame = 1.0 / 60.0;

TEST(Cycle, Basics)
{
  auto cycle = GraphManager::createCycle();

	cycle->setFrom(0.0f);
	cycle->setTo(1.0f);
	cycle->setMultiplier(0.1);

  int expectedUpdates = (int)((cycle->getTo() / sPerFrame) / cycle->getMultiplier()) + 2;
  {
		// Cycle is not running.
    for (int i = 1; i < expectedUpdates; ++i)
    {
      GraphManager::update(sPerFrame);
    }

    float delta = cycle->getTo() - cycle->getData().getFloat();
    EXPECT_TRUE(Math::eq(delta, cycle->getTo()));
	}
  {
		cycle->play();

    float lastValue = 0.0f;
    float currentValue = 0.0f;
    for (int i = 0; i < expectedUpdates; ++i)
    {
			lastValue = cycle->getData().getFloat();
      GraphManager::update(sPerFrame);
			currentValue = cycle->getData().getFloat();

			if (i + 1 != expectedUpdates)
			  EXPECT_TRUE(lastValue < currentValue);
    }

		// Check if cycle finished
    EXPECT_TRUE(lastValue > currentValue);
  }
}
