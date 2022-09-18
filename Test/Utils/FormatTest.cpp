#include "gtest/gtest.h"

#include "Utils/Format.h"

TEST(FormatTest, NaN)
{
	auto strVal = Utils::toString(std::numeric_limits<float>::quiet_NaN());
	EXPECT_TRUE(strVal == ".NaN");
}
