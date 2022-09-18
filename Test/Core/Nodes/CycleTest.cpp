#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"
#include "Utils.h"

using namespace Core;

double sPerFrame = 1.0 / 60.0;

TEST_F(CycleTestFixture, CycleIsUpdatetedAndResetAfterMaxValueIsGained)
{
	auto cycle = GraphManager::createCycle();

	cycle->setFrom(0.0f);
	cycle->setTo(1.0f);
	cycle->setMultiplier(0.1f);

	int expectedUpdates =
	    (int)((cycle->getTo() / sPerFrame) / cycle->getMultiplier()) + 2;
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

TEST_F(CycleTestFixture, TriggerStepNextOnParentPlay)
{
	auto cycle1 = GraphManager::createCycle();
	auto cycle2 = GraphManager::createCycle();

	GraphManager::plug(cycle1, cycle2, I3T_CYCLE_OUT_PLAY, I3T_CYCLE_IN_NEXT);

	cycle2->play();
	for (int i = 0; i < 25; ++i)
	{
		GraphManager::update(sPerFrame);
	}
	cycle2->pause();

	float previous = cycle2->getData().getFloat();
	cycle1->play();

	// cycle6 value increases by step on parent play.
	float expected = previous + cycle2->getManualStep();
	float current = cycle2->getData().getFloat();
	EXPECT_TRUE(Math::eq(expected, current));
}

TEST_F(CycleTestFixture, TriggerOnPlay)
{
	auto cycle1 = GraphManager::createCycle();

	auto cycle2 = GraphManager::createCycle();
	auto cycle3 = GraphManager::createCycle();
	auto cycle4 = GraphManager::createCycle();
	auto cycle5 = GraphManager::createCycle();
	auto cycle6 = GraphManager::createCycle();
	auto cycle7 = GraphManager::createCycle();

	cycle3->play();
	cycle4->play();
	cycle5->play();
	cycle6->play();

	for (int i = 0; i < 25; ++i)
	{
		GraphManager::update(sPerFrame);
	}

	{
		auto plugResult = GraphManager::plug(cycle1, cycle2, I3T_CYCLE_OUT_PLAY,
		                                     I3T_CYCLE_IN_PLAY);
		EXPECT_EQ(ENodePlugResult::Ok, plugResult);
	}
	{
		auto plugResult = GraphManager::plug(cycle1, cycle3, I3T_CYCLE_OUT_PLAY,
		                                     I3T_CYCLE_IN_PAUSE);
		EXPECT_EQ(ENodePlugResult::Ok, plugResult);
	}
	{
		auto plugResult = GraphManager::plug(cycle1, cycle4, I3T_CYCLE_OUT_PLAY,
		                                     I3T_CYCLE_IN_STOP);
		EXPECT_EQ(ENodePlugResult::Ok, plugResult);
	}
	{
		auto plugResult = GraphManager::plug(cycle1, cycle5, I3T_CYCLE_OUT_PLAY,
		                                     I3T_CYCLE_IN_PREV);
		EXPECT_EQ(ENodePlugResult::Ok, plugResult);
	}
	{

		auto plugResult = GraphManager::plug(cycle1, cycle6, I3T_CYCLE_OUT_PLAY,
		                                     I3T_CYCLE_IN_NEXT);
		EXPECT_EQ(ENodePlugResult::Ok, plugResult);
	}

	float cycle5Val = cycle5->getData().getFloat();
	float cycle6Val = cycle6->getData().getFloat();

	cycle1->play();
	// cycle2 plays on parent play.
	EXPECT_TRUE(cycle2->isRunning());

	// cycle3 pauses on parent play.
	EXPECT_FALSE(cycle3->isRunning());

	// cycle4 pauses and resets on parent play.
	EXPECT_FALSE(cycle4->isRunning());

	// \todo MH
	// EXPECT_TRUE(Math::eq(cycle4->getFrom(), cycle4->getData().getFloat()));

	{
		// cycle5 value decreases by step on parent play.
		float expected = cycle5Val - cycle5->getManualStep();
		float current = cycle5->getData().getFloat();
		// \todo MH
		// EXPECT_TRUE(Math::eq(expected, current));
	}
	{
		// cycle6 value increases by step on parent play.
		float expected = cycle6Val + cycle6->getManualStep();
		float current = cycle6->getData().getFloat();
		EXPECT_TRUE(Math::eq(expected, current));
	}
}
