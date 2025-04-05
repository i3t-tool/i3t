#include "gtest/gtest.h"

#include "DIWNE/Core/diwne_imgui.h"
#include "DIWNE/Core/diwne_utils.h"

using namespace DIWNE;

TEST(UtilsTest, ClampTest)
{
	EXPECT_FLOAT_EQ(DMath::clamp(0.0f, 0.0f, 1.0f), 0.0f);
	EXPECT_FLOAT_EQ(DMath::clamp(1.f, 0.f, 1.f), 1.f);
	EXPECT_FLOAT_EQ(DMath::clamp(5.f, 1.f, 10.f), 5.f);
	EXPECT_FLOAT_EQ(DMath::clamp(-1.f, 0.f, 10.f), 0.f);
	EXPECT_FLOAT_EQ(DMath::clamp(15.f, 0.f, 10.f), 10.f);

	EXPECT_EQ(DMath::clamp(1, 0, 1), 1);
	EXPECT_EQ(DMath::clamp(5, 1, 10), 5);
	EXPECT_EQ(DMath::clamp(-1, 0, 10), 0);
	EXPECT_EQ(DMath::clamp(15, 0, 10), 10);

	EXPECT_DOUBLE_EQ(DMath::clamp(15.2, -15.4, -1.0), -1.0);
	EXPECT_DOUBLE_EQ(DMath::clamp(-20.0, -15.4, -1.0), -15.4);
	EXPECT_DOUBLE_EQ(DMath::clamp(-15.3, -15.4, 20.4), -15.3);

	auto ret1 = DMath::clamp(0, 0, 1);
	EXPECT_EQ(typeid(int), typeid(ret1));

	auto ret2 = DMath::clamp(0.0, 0.0, 1.0);
	EXPECT_EQ(typeid(double), typeid(ret2));

	auto ret3 = DMath::clamp(0.f, 0.f, 1.f);
	EXPECT_EQ(typeid(float), typeid(ret3));
}

TEST(UtilsTest, MapTest)
{
	auto ret = DMath::map(5.f, 0.f, 10.f, -100.f, 100.f);
	EXPECT_FLOAT_EQ(ret, 0.0f);
	EXPECT_EQ(typeid(float), typeid(ret));

	EXPECT_FLOAT_EQ(DMath::map(0, 0, 10, 0, 100), 0.0f);
	EXPECT_FLOAT_EQ(DMath::map(10, 0, 10, 0, 100), 100.0f);
}

TEST(UtilsTest, LenTest)
{
	EXPECT_FLOAT_EQ(DMath::len(ImVec2(3, 4)), 5.0f);
	EXPECT_FLOAT_EQ(DMath::len(ImVec2(0, 0)), 0.0f);
}

TEST(UtilsTest, NormalizeTest)
{
	ImVec2 norm = DMath::normalize(ImVec2(3, 4));
	EXPECT_NEAR(norm.x, 0.6f, 0.0001f);
	EXPECT_NEAR(norm.y, 0.8f, 0.0001f);
}

TEST(UtilsTest, SmoothStepTest)
{
	EXPECT_FLOAT_EQ(DMath::smoothstep(0.0f, 1.0f, -1.0f), 0.0f);
	EXPECT_FLOAT_EQ(DMath::smoothstep(0.0f, 1.0f, 0.5f), 0.5f * 0.5f * (3.0f - 2.0f * 0.5f));
	EXPECT_FLOAT_EQ(DMath::smoothstep(0.0f, 1.0f, 1.5f), 1.0f);

	EXPECT_FLOAT_EQ(DMath::smoothstep(1.2f, 4.24f, 2.73f), 0.50493419f);
}

TEST(UtilsTest, EqualsFloatTest)
{
	EXPECT_TRUE(DMath::equals(0.1f + 0.2f, 0.3f, 0.0001f));
	EXPECT_FALSE(DMath::equals(0.1f, 0.2f, 0.0001f));
}

TEST(UtilsTest, EqualsVec2Test)
{
	EXPECT_TRUE(DMath::equals(ImVec2(1.0001f, 2.0001f), ImVec2(1.0002f, 2.0002f), 0.0002f));
	EXPECT_FALSE(DMath::equals(ImVec2(1.0f, 2.0f), ImVec2(1.1f, 2.1f), 0.0001f));
}

TEST(UtilsTest, ModTest)
{
	EXPECT_FLOAT_EQ(DMath::mod(5.5f, 2.0f), 1.5f);

	auto ret = DMath::mod(ImVec2(5.5f, 7.3f), ImVec2(2.0f, 3.0f));
	EXPECT_FLOAT_EQ(ret.x, 1.5f);
	EXPECT_FLOAT_EQ(ret.y, 1.3f);
	ret = DMath::mod(ImVec2(-5.5f, -7.3f), ImVec2(2.0f, 3.0f));
	EXPECT_FLOAT_EQ(ret.x, 0.5f);
	EXPECT_FLOAT_EQ(ret.y, 1.7f);
	ret = DMath::mod(ImVec2(-5.5f, -7.3f), ImVec2(-2.0f, -3.0f));
	EXPECT_FLOAT_EQ(ret.x, -1.5f);
	EXPECT_FLOAT_EQ(ret.y, -1.3f);
	ret = DMath::mod(ImVec2(-91.7f, 23.41f), ImVec2(100.0f, 20.0f));
	EXPECT_FLOAT_EQ(ret.x, 8.3f);
	EXPECT_FLOAT_EQ(ret.y, 3.41f);
}
