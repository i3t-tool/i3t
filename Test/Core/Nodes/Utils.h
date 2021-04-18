#pragma once

#include "gtest/gtest.h"

#include "glm/glm.hpp"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

template <typename T> inline void setValue_expectOk(NodePtr node, T&& value)
{
	auto result = node->setValue(std::forward<T>(value));
	EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
}

template <typename T> inline void setValue_expectOk(NodePtr node, T&& value, glm::ivec2 coords)
{
  auto result = node->setValue(std::forward<T>(value), coords);
  EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
}

template <typename T1, typename T2>
inline void plug_expectOk(T1&& lhs, T2&& rhs, int leftIndex, int rightIndex)
{
	auto plugResult = GraphManager::plug(std::forward<T1>(lhs), std::forward<T2>(rhs), leftIndex, rightIndex);
	EXPECT_EQ(ENodePlugResult::Ok, plugResult);
}
