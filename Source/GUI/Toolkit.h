#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "imgui.h"

#include "Core/API.h"

namespace GUI
{
bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback,
               void* user_data);

void ToggleButton(const char* label, bool& toggled, ImVec2 size = ImVec2(0.0f, 0.0f));

glm::vec2 imToGlm(const ImVec2& v);
glm::vec4 imToGlm(const ImVec4& v);
ImVec2 glmToIm(const glm::vec2& v);
ImVec4 glmToIm(const glm::vec4& v);

inline glm::vec2 convertCoordinates(glm::vec2 position, glm::vec2 newOrigin) { return position - newOrigin; }

void drawCross(glm::vec2 pos, ImDrawList* drawList, float thickness, float size, ImColor color);

void drawEllipse(float cx, float cy, float rx, float ry, int num_segments, ImDrawList* drawList, ImColor color,
                 float thickness = 1.0f);

} // namespace GUI
