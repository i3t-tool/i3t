#include "gtest/gtest.h"

#include "DIWNE/Core/Style/DiwneStyle.h"
#include "DIWNE/diwne_include.h"

using namespace DIWNE;

bool imVec4Eq(const ImVec4& a, const ImVec4& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

TEST(StyleTest, BasicStyleTest)
{
	DiwneStyle style;
	ImVec4 test = style.color(DiwneStyle::nodeBg);
	const ImVec4& test2 = style.color(DiwneStyle::nodeBg);
	const ImVec4& test3 = style.color(DiwneStyle::nodeBg);
	ASSERT_TRUE(&test2 == &test3);
	ASSERT_TRUE(imVec4Eq(test2, test3));
	ASSERT_TRUE(imVec4Eq(test2, test));
}