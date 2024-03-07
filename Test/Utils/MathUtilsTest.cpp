/**
 * \file
 * \brief
 * \authors Petr Felkel, Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
// #include "Common.h"
#include "Common.h"
#include "glm/gtx/euler_angles.hpp"
#include "gtest/gtest.h"

#include "Generator.h"
#include "Utils/Math.h"


TEST(MathUtilsTest, QuatLerp)
{
	auto lhs = glm::mat4(1.0f);
	auto rhs = glm::eulerAngleX(glm::radians(180.0f));

	std::array alphas = {0.0f, 0.25f, 0.50f, 0.75f, 1.0f};
	std::array angles = {0.0f, 45.0f, 90.0f, 135.0f, 180.0f};

	// add random translation
	auto lhsT = generateVec4();
	auto rhsT = generateVec4();
	lhsT[3] = 1.0;
	rhsT[3] = 1.0;

	lhs[3] = lhsT;
	rhs[3] = rhsT;

	for (int i = 0; i < 5; ++i)
	{
		auto result = Math::lerp(lhs, rhs, alphas[i], true); // true => use quaternion interpolation

		auto expected = glm::eulerAngleX(glm::radians(angles[i])); // rotation part
		expected[3] = glm::mix(lhsT, rhsT, alphas[i]);             // translation part

		// std::cout << "Ti = " << Utils::toString(expected[3]) << std::endl;

		EXPECT_TRUE(Math::eq(expected, result, Math::FLT_EPSILON_FIVE_DECIMALS));
		// EXPECT_PRED_FORMAT2(AssertRoughlyEqualMatrices, expected, result, Math::FACTOR_ROUGHLY_SIMILAR);
	}
}

// test of comparison assert from common.h
TEST(MathUtilsTest, MatrixComparison)
{
	const glm::mat4 a = generateMat4();
	const glm::mat4 b = generateMat4();
	EXPECT_PRED_FORMAT2(AssertEqualMatrices, a, a);  // Succeeds
	EXPECT_PRED_FORMAT2(!AssertEqualMatrices, a, b); // !Fails
}
