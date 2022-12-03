#pragma once

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

inline void Text(const char* text, EFont font) { Text(text, I3T::getFont(font)); }

inline bool Button(const char* id, bool disabled = false)
{
	if (disabled)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}

	const auto result = ImGui::Button(id);

	if (disabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	return result;
}
} // namespace GUI
