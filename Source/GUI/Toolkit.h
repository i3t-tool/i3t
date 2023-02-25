#pragma once

#include <string>

#include "glm/glm.hpp"
#include "imgui.h"

namespace GUI
{
bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback,
               void* user_data);

glm::vec2 imToGlm(const ImVec2& v);
glm::vec4 imToGlm(const ImVec4& v);
ImVec2 glmToIm(const glm::vec2& v);
ImVec4 glmToIm(const glm::vec4& v);

inline glm::vec2 convertCoordinates(glm::vec2 position, glm::vec2 newOrigin) { return position - newOrigin; }

void drawCross(glm::vec2 pos, ImDrawList* drawList, float thickness, float size, ImColor color);

} // namespace GUI
