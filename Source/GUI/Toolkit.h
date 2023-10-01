#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui.h"
#include "imgui_internal.h"

#include "API.h"

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

bool Button(const char* id, bool disabled = false);

bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback,
               void* user_data);

void ToggleButton(const char* label, bool& toggled, ImVec2 size = ImVec2(0.0f, 0.0f));

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

void drawCross(glm::vec2 pos, ImDrawList* drawList, float thickness, float size, ImColor color);

void drawEllipse(float cx, float cy, float rx, float ry, int num_segments, ImDrawList* drawList, ImColor color,
                 float thickness = 1.0f);

} // namespace GUI
