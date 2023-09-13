// #include "Common.h"
#include "glm/gtx/euler_angles.hpp"
#include "gtest/gtest.h"

#include "Generator.h"
#include "Utils/Format.h"
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
		std::cout << "Angle[" << i << "] = " << angles[i] << std::endl;

		auto result = Math::lerp(lhs, rhs, alphas[i], true); // true => use quaternion interpolation

		auto expected = glm::eulerAngleX(glm::radians(angles[i])); // rotation part
		expected[3] = glm::mix(lhsT, rhsT, alphas[i]);             // translation part

		// std::cout << "Ti = " << Utils::toString(expected[3]) << std::endl;

		EXPECT_TRUE(Math::eq(expected, result, Math::FLT_EPSILON_FIVE_DECIMALS));
		// EXPECT_PRED_FORMAT2(AssertRoughlyEqualMatrices, expected, result, Math::FACTOR_ROUGHLY_SIMILAR);
	}
}
