#include "gtest/gtest.h"

#include "DIWNE/Core/Style/Style.h"
#include "DIWNE/diwne_include.h"

using namespace DIWNE;

bool imVec4Eq(const ImVec4& a, const ImVec4& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

TEST(StyleTest, BasicStyleTest)
{
	StyleBase style;
	ImVec4 test = style.color(Style::NODE_BG);
	const ImVec4& test2 = style.color(Style::NODE_BG);
	const ImVec4& test3 = style.color(Style::NODE_BG);
	ASSERT_TRUE(&test2 == &test3);
	ASSERT_TRUE(imVec4Eq(test2, test3));
	ASSERT_TRUE(imVec4Eq(test2, test));
}

// TODO: Add multiple Styles test and editor style assignment
// TODO: Test Style copying (copy constr + copy assign operator)
// TODO: Test Style moving (move constr + move operator)