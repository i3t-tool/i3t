#pragma once

#include "imgui.h"

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
}
