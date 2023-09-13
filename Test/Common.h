#pragma once

#include "Utils/Math.h" // FACTOR_FOR_MATRICES

#include <gtest/gtest.h>
// #include "glm/matrix.hpp"

// bool compare(const glm::mat4& lhs, const glm::mat4& rhs, float epsilon = 0.001f);
bool compare(const glm::mat4& lhs, const glm::mat4& rhs, int factor = Math::FACTOR_SIMILAR);
testing::AssertionResult AssertEqualMatrices(const char* m_expr, const char* n_expr, glm::mat4 m, glm::mat4 n);
testing::AssertionResult AssertRoughlyEqualMatrices(const char* m_expr, const char* n_expr, glm::mat4 m, glm::mat4 n);
