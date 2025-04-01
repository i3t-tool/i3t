#pragma once

#include "diwne_imgui.h"

#define DIWNE_PIXEL_EPSILON 0.001f

#define DIWNE_TRUNC(val) IM_TRUNC(val)
#define DIWNE_TRUNC_VEC(val) ImVec2(IM_TRUNC(val.x), IM_TRUNC(val.y))
#define DIWNE_TRUNC_POS() DIWNE::DGui::truncScreenCursorPos()

// #define DIWNE_MIN_TRUNC(_VAL) ((float) (int) (_VAL < 0 ? _VAL - 1 : _VAL))
// #define DIWNE_MIN_TRUNC_VEC() ImVec2(DIWNE_MIN_TRUNC(val.x), DIWNE_MIN_TRUNC(val.y))

namespace DIWNE
{
namespace DGui
{
inline ImVec2 truncScreenCursorPos()
{
	// Snaps the screen cursor position to lower pixel boundary
	// The idea is that to properly measure size of ImGui elements (ImGui groups),
	// we need to ensure they begin at a rounded position, to make sure their new line begins at the same position.
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImGui::SetCursorScreenPos(DIWNE_TRUNC_VEC(pos));
	return pos - ImGui::GetCursorScreenPos(); // Returns the error
}
inline void BeginGroup()
{
	// We align cursor position to pixel boundaries BEFORE starting the group
	// This ensures the group measures its size correctly
	// See https://github.com/ocornut/imgui/issues/8527
	DIWNE_TRUNC_POS();
	ImGui::BeginGroup();
}
/// Ensures a new ImGui line and moves the cursor so that no vertical item spacing is applied.
inline void NewLine()
{
	if (ImGui::GetCurrentWindowRead()->DC.IsSameLine)
		ImGui::NewLine();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
}
inline void SameLineDummy(const ImVec2& size)
{
	ImGui::Dummy(size);
	ImGui::SameLine(0, 0);
}
/// Dummy that effectively moves the cursor by exactly the passed size in the X and Y direction.
inline void DummyXY(const ImVec2& size)
{
	ImGui::Dummy(size);
	ImGui::SetCursorScreenPos(ImGui::GetItemRectMax());
}
/// Dummy that begins at the max cursor position. Expands current content by the size and starts a new line.
inline void DummyMax(const ImVec2& size)
{
	ImGui::SetCursorScreenPos(ImGui::GetCurrentWindowRead()->DC.CursorMaxPos);
	ImGui::Dummy(size);
}
inline void BeginVerticalAlign(float yOffset)
{
	assert(yOffset > 0.0f);
	ImGui::BeginGroup();
	DummyXY(ImVec2(0.0f, yOffset));
}
inline void EndVerticalAlign(float yOffset)
{
	NewLine();
	if (yOffset > 0.0f)
		DummyXY(ImVec2(0.0f, yOffset));
	ImGui::EndGroup();
}
/// Variant of ImGui::AlignTextToFramePadding for custom frame padding
inline void AlignTextToPadding(ImVec2 padding)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	window->DC.CurrLineSize.y = ImMax(window->DC.CurrLineSize.y, g.FontSize + padding.y * 2);
	window->DC.CurrLineTextBaseOffset = ImMax(window->DC.CurrLineTextBaseOffset, padding.y);
}
/// Variant of ImGui::GetFrameHeight() for custom frame padding
inline float GetFrameHeight(ImVec2 padding)
{
	ImGuiContext& g = *GImGui;
	return g.FontSize + padding.y * 2.0f;
}
} // namespace DGui
namespace DUtils
{
inline bool equals(const ImVec2& a, const ImVec2& b)
{
	return a.x == b.x && a.y == b.y;
}
inline bool equals(const ImRect& a, const ImRect& b)
{
	return equals(a.Min, b.Min) && equals(a.Max, b.Max);
}
inline bool equals(float val1, float val2, float abs_error)
{
	return fabsf(val1 - val2) <= abs_error;
}
inline bool equals(const ImVec2& a, const ImVec2& b, float abs_error)
{
	return equals(a.x, b.x, abs_error) && equals(a.y, b.y, abs_error);
}
} // namespace DUtils

namespace DDraw
{
void drawSpringDebug(const ImVec2& pos, ImVec2 springSize, bool horizontal);
} // namespace DDraw
} // namespace DIWNE