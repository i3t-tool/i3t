/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "I3T.h"

namespace GUI
{
inline void Text(const char* text, ImFont* font)
{
	ImGui::PushFont(font);
	ImGui::TextUnformatted(text);
	ImGui::PopFont();
}

inline void Text(const char* text, EFont font)
{
	Text(text, I3T::getFont(font));
}

inline void TextCentered(const char* text, ImRect rect, ImU32 col)
{
	ImVec2 textSize = ImGui::CalcTextSize(text);
	ImVec2 center = ImVec2(rect.Min.x + rect.GetWidth() / 2, rect.Min.y + rect.GetHeight() / 2);
	ImVec2 textOrigin = center - ImVec2(textSize.x / 2, textSize.y / 2);
	ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), textOrigin, col, text);
}

bool Button(const char* id, bool disabled = false);

bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback,
               void* user_data);

bool ToggleButton(const char* label, bool& toggled, bool invert = false, ImVec2 size = ImVec2(0.0f, 0.0f));

bool ButtonWithCorners(const char* label, ImDrawFlags corners, const ImVec2& size = ImVec2(0, 0));

bool ButtonWithCornersEx(const char* label, ImDrawFlags corners, const ImVec2& size_arg = ImVec2(0, 0),
                         ImGuiButtonFlags flags = 0);

// Render a rectangle shaped with optional rounding, corners and borders
void RenderFrameWithCorners(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border, float rounding,
                            ImDrawFlags corners);

inline ImVec2 floorImVec2(const ImVec2& v)
{
	return ImVec2((float) (int) v.x, (float) (int) v.y);
}

inline glm::vec2 imToGlm(const ImVec2& v)
{
	return glm::vec2(v.x, v.y);
}

inline glm::vec4 imToGlm(const ImVec4& v)
{
	return glm::vec4(v.x, v.y, v.z, v.w);
}

inline ImVec2 glmToIm(const glm::vec2& v)
{
	return ImVec2(v.x, v.y);
}

inline ImVec4 glmToIm(const glm::vec4& v)
{
	return ImVec4(v.x, v.y, v.z, v.w);
}

inline glm::vec2 convertCoordinates(glm::vec2 position, glm::vec2 newOrigin)
{
	return position - newOrigin;
}

void startVerticalAlign(float yOffset);

void endVerticalAlign();

void drawCross(glm::vec2 pos, ImDrawList* drawList, float thickness, float size, ImColor color);

void drawEllipse(float cx, float cy, float rx, float ry, int num_segments, ImDrawList* drawList, ImColor color,
                 float thickness = 1.0f);

inline void Tooltip(const char* header, const char* description)
{
	ImGui::BeginTooltip();
	ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
	ImGui::TextUnformatted(header);

	if (strlen(description) != 0)
	{
		ImGui::Dummy({0.0f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_Text, {0.7f, 0.7f, 0.7f, 1.0f});
		ImGui::TextUnformatted(description);
		ImGui::PopStyleColor();
	}

	ImGui::PopTextWrapPos();
	ImGui::EndTooltip();
}

/**
 * Sets style for the tab of a dockable window.
 * This method should be called after the ImGui::Begin call of a dockable window.
 * Style needs to be popped afterwards with dockTabStylePop()
 * Note that this style affects regular ImGui tabs,
 * the style needs to be popped before using those in the dockable window.
 */
void dockTabStylePush();
void dockTabStylePop();

} // namespace GUI
