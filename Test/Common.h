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
#pragma once

#include "Utils/Math.h" // FACTOR_FOR_MATRICES

#include <gtest/gtest.h>
// #include "glm/matrix.hpp"

// bool compare(const glm::mat4& lhs, const glm::mat4& rhs, float epsilon = 0.001f);
bool compare(const glm::mat4& lhs, const glm::mat4& rhs, int factor = Math::FACTOR_SIMILAR);
testing::AssertionResult AssertEqualMatrices(const char* m_expr, const char* n_expr, glm::mat4 m, glm::mat4 n);
testing::AssertionResult AssertRoughlyEqualMatrices(const char* m_expr, const char* n_expr, glm::mat4 m, glm::mat4 n);
testing::AssertionResult assertionCompare(const glm::mat4& lhs, const glm::mat4& rhs, int factor);
testing::AssertionResult assertionCompare(const glm::quat& lhs, const glm::quat& rhs, int factor);
