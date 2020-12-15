//
// Created by Martin on 24.10.2020.
//
#pragma once

#include "gtest/gtest.h"

#include <Commands/ApplicationCommands.h>

bool g_listenerCalled = false;
void dummyListener()
{
  g_listenerCalled = true;
}

TEST(addCommandListenerTest, commandWillBeProcessed)
{
  EXPECT_TRUE(g_listenerCalled);
}
