#include "glm/gtx/euler_angles.hpp"
#include "gtest/gtest.h"

#include "Utils/Math.h"

TEST(MathUtilsTest, QuatLerp)
{
	auto lhs = glm::mat4(1.0f);
	auto rhs = glm::eulerAngleX(glm::radians(180.0f));

	std::array alphas = {0.25f, 0.50f, 0.75f};
	std::array angles = {45.0f, 90.0f, 135.0f};

	for (int i = 0; i < 3; ++i)
	{
		auto result = Math::lerp(lhs, rhs, alphas[i], true);

		auto expected = glm::eulerAngleX(glm::radians(angles[i]));
		EXPECT_TRUE(Math::eq(expected, result));
	}
}
