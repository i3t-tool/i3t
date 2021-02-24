/**
 * \file	GUI/style.h
 *
 * Declares the tab group class
 */
#pragma once

#include <imgui.h>

/// Color for tab background and title.
#define IM_TAB_COLOR    ImVec4(0.278f, 0.278f, 0.286f, 1.00f);
#define IM_ACTIVE_COLOR ImVec4(0.259f, 0.588f, 0.980f, 1.00f);

/**
 * Setup global style based on Zadina's thesis.
 */
inline void setupImGuiStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.439f, 0.439f, 0.455f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = IM_TAB_COLOR style.Colors[ImGuiCol_TitleBgActive] =
			IM_TAB_COLOR style.Colors[ImGuiCol_TitleBgCollapsed] = IM_TAB_COLOR style.Colors[ImGuiCol_MenuBarBg] =
					ImVec4(0.40f, 0.38f, 0.369f, 1.00f);

	style.Colors[ImGuiCol_Tab] = IM_TAB_COLOR style.Colors[ImGuiCol_TabActive] =
			IM_ACTIVE_COLOR style.Colors[ImGuiCol_TabUnfocused] = IM_TAB_COLOR style.Colors[ImGuiCol_TabUnfocusedActive] =
					IM_TAB_COLOR

							style.Colors[ImGuiCol_Separator] = IM_TAB_COLOR

	                                                   style.Colors[ImGuiCol_PlotHistogram] = IM_ACTIVE_COLOR

	                                                                                              style.FramePadding.y =
							8.0f;
}
