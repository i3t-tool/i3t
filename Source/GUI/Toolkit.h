#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include "Core/API.h"

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
