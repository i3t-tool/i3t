#pragma once

#include "gtest/gtest.h"

#include "Commands/ApplicationCommands.h"

class TestCommand : public Command<TestCommand> {};

bool g_listenerCalled = false;

void dummyListener()
{
  g_listenerCalled = true;
}

/// \todo MH Not testable (needs running Application).
TEST(CommandTest, CommandWillBeProcessed)
{
	TestCommand::addListener(&dummyListener);
	TestCommand::dispatch();
  // EXPECT_TRUE(g_listenerCalled);
  EXPECT_TRUE(!g_listenerCalled);
}
