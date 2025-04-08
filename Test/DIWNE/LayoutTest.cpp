#include "gtest/gtest.h"

#include "DIWNE/diwne_include.h"

#include <gmock/gmock-matchers.h>

using namespace DIWNE;

#include "Util/DiwneTestApp.h"

class LayoutTest : public ::testing::Test
{
public:
	DIWNE::SettingsDiwne settings;
	std::shared_ptr<NodeEditor> editor;

	std::unique_ptr<DiwneTestApp> app;

	void SetUp() override
	{
		editor = std::make_shared<NodeEditor>("NodeEditor", &settings);
		app = std::make_unique<DiwneTestApp>();
		app->create(editor.get());
	}

	void TearDown() override
	{
		app->destroy();
	}
};

void twoByTwoButtonGroup()
{
	ImGui::BeginGroup();
	ImGui::Button("e", ImVec2(100, 100));
	ImGui::SameLine();
	ImGui::Button("e", ImVec2(100, 100));
	ImGui::Button("e", ImVec2(100, 100));
	ImGui::SameLine();
	ImGui::Button("e", ImVec2(100, 100));
	ImGui::EndGroup();
}

void twoByTwoButtonGroupWithWiderSecondLine()
{
	ImGui::BeginGroup();
	ImGui::Button("e", ImVec2(100, 100));
	ImGui::SameLine();
	ImGui::Button("e", ImVec2(100, 100));
	ImGui::Button("e", ImVec2(110, 100));
	ImGui::SameLine();
	ImGui::Button("e", ImVec2(110, 100));
	ImGui::EndGroup();
}

TEST_F(LayoutTest, ImGuiFractionalSizeMeasuring)
{
	app->setTestCallback([]() {
		ImRect itemRect;

		ImGui::Button("Test button", ImVec2(100, 100));
		ImVec2 realSize = ImGui::GetItemRectSize();
		assert(DMath::equals(realSize, ImVec2(100, 100)));

		// Testing ImGui cursor behavior with a fractional/unrounded screen cursor position.

		// The issue here is that cursor position is not initially rounded by ImGui, not for the whole first line.
		// But the moment the cursor is moved to a new line (ImGui::SameLine() isn't called), the cursor position
		// gets rounded/truncated to the nearest integer.
		// When width is measured in a group, the original starting X position of the "group" will be the orignal
		// untruncated position.

		// This means the items on the second line begin "earlier", which doesn't cause issues when they are the same
		// size as items on line 1, as the higher width of the elements on the first line will be reported.
		// But if the items on line 2 are bigger, then the reported width will be smaller by the fractional (< 1px)
		// offset they started earlier by.

		// This issue is inverse when the coordinates are negative.
		// In that case truncation snaps to the other side, which means the items on the second line begin "later" and
		// expand the entire group width even if they are the same width as the first line.

		// Either way this means that measured group sizes might actually differ depending on at which exact fractional
		// X/Y position they started. The error margin is < 1px, but can cause issues when size are measured for DIWNE
		// layout springs, because springs will try to compensate for the changing element size in a never ending loop,
		// making the springs oscillate by 1 px.

		// In DIWNE, fractional positions are very common and unavoidable since DIWNE scales all ImGui style variables
		// without rounding them, so that at small zoom levels they don't all degrade to 0.

		// Uglify the cursor position and item spacing
		float uglyNum1 = 123.324823992f;
		float uglyNum2 = 123.9097823992f;
		ImGui::GetStyle().ItemSpacing.x = 10.7635210389f;

		// Two same size buttons on a single line, reported size is correct
		ImGui::SetCursorScreenPos(ImVec2(uglyNum1, 100.0f));

		ImGui::BeginGroup();
		ImGui::Button("e", ImVec2(100, 100));
		ImGui::SameLine();
		ImGui::Button("e", ImVec2(100, 100));
		ImGui::EndGroup();

		itemRect = ImGui::GetCurrentContext()->LastItemData.Rect;
		EXPECT_NEAR(itemRect.GetSize().x, 210.7635210389, 0.001f);

		// Adding two same size buttons below causes no problems when screen cursor pos is positive
		ImGui::SetCursorScreenPos(ImVec2(uglyNum1, 100.0f));

		twoByTwoButtonGroup();

		itemRect = ImGui::GetCurrentContext()->LastItemData.Rect;
		EXPECT_NEAR(itemRect.GetSize().x, 210.7635210389, 0.001f);

		// But when the screen cursor X position is negative, the size is now bigger by about 0.3 pixels
		ImGui::SetCursorScreenPos(ImVec2(-uglyNum1, 100.0f));

		twoByTwoButtonGroup();

		itemRect = ImGui::GetCurrentContext()->LastItemData.Rect;
		ASSERT_THAT(itemRect.GetSize().x, testing::Not(testing::FloatNear(210.7635210389, 0.001f)));
		EXPECT_NEAR(itemRect.GetSize().x, 211.088348, 0.001f);

		// The issue can be resolved by truncating the screen cursor position manually before starting the group
		ImGui::SetCursorScreenPos(ImVec2(-uglyNum1, 100.0f));

		DIWNE_TRUNC_POS();
		twoByTwoButtonGroup();

		itemRect = ImGui::GetCurrentContext()->LastItemData.Rect;
		EXPECT_NEAR(itemRect.GetSize().x, 210.7635210389, 0.001f);

		// Similarly issue arises with positive X screen cursor pos when the second line is wider than the first
		// First we try with a truncated position
		ImGui::SetCursorScreenPos(ImVec2(100.0f, 100.0f));

		twoByTwoButtonGroupWithWiderSecondLine();

		itemRect = ImGui::GetCurrentContext()->LastItemData.Rect;
		EXPECT_NEAR(itemRect.GetSize().x, 230.7635210389, 0.001f);

		// Then with a untruncated one, causing the size to drop by about 0.3 pixels
		ImGui::SetCursorScreenPos(ImVec2(uglyNum1, 100.0f));

		twoByTwoButtonGroupWithWiderSecondLine();

		itemRect = ImGui::GetCurrentContext()->LastItemData.Rect;
		ASSERT_THAT(itemRect.GetSize().x, testing::Not(testing::FloatNear(230.7635210389, 0.001f)));
		EXPECT_NEAR(itemRect.GetSize().x, 230.43869, 0.001f);
	});
	app->frame();
}
