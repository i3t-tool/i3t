#pragma once

#include "diwne_imgui.h"

#define DIWNE_TRUNC(val) IM_TRUNC(val)
#define DIWNE_TRUNC_VEC(val) ImVec2(IM_TRUNC(val.x), IM_TRUNC(val.y))
#define DIWNE_TRUNC_POS() DIWNE::DGui::truncScreenCursorPos()

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
	if (ImGui::GetCurrentWindow()->DC.IsSameLine)
		ImGui::NewLine();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
}
inline void SameLineDummy(const ImVec2& size)
{
	ImGui::Dummy(size);
	ImGui::SameLine(0, 0);
}
inline bool equals(const ImVec2& a, const ImVec2& b)
{
	return a.x == b.x && a.y == b.y;
}
inline bool equals(const ImRect& a, const ImRect& b)
{
	return equals(a.Min, b.Min) && equals(a.Max, b.Max);
}
} // namespace DGui
} // namespace DIWNE