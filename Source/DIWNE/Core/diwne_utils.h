#pragma once

#include "diwne_imgui.h"

namespace DIWNE
{
namespace DGui
{
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
} // namespace DGui
} // namespace DIWNE