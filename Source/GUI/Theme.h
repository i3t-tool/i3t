/**
 * \file	GUI/style.h
 *
 * Declares the tab group class
 */
#pragma once

#include "imgui.h"

#include <array>
#include <map>

#include "Core/Defs.h"

/// Color for tab background and title.
#define IM_TAB_COLOR    ImVec4(0.278f, 0.278f, 0.286f, 1.00f);
#define IM_ACTIVE_COLOR ImVec4(0.259f, 0.588f, 0.980f, 1.00f);

enum class EColor
{
  Text,
	Border,
	WindowBackground,
	PopupBackground,
	MenuBarBackground,
	PrimaryColor, /// Color of tabs, separators, titles ...
  ActiveColor,
	TabColor
};

enum class EFont
{
	MenuLarge,
	MenuSmall,
	TutorialText,
	Button,
	Tab,
	Node,
	Title,
	TaskTitle,
};

/**
 * Global color scheme.
 */
class Theme
{
  using Colors = std::map<EColor, ImVec4>;
  Colors m_colors;
	std::map<EFont, size_t> m_fontsAssoc;

	static constexpr const size_t m_fontsCount = 4;
  /// \todo Set dynamic scale (reload font in runtime).
  static constexpr float m_fontScale = 1.2f;
  std::array<ImFont*, m_fontsCount + 1> m_fonts = {nullptr, nullptr, nullptr, nullptr, nullptr};

public:
	/**
	 * Creates default color scheme.
	 */
	Theme();
	Theme(const Colors& colors) { m_colors = colors; }

	void init();

	/**
	 * Call this function whenever you change style settings.
	 */
	void apply();
	ImVec4 get(EColor color)
	{
		if (m_colors.count(color) == 0)
			return ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };

		return m_colors[color];
	}

  ImFont* get(EFont font)
	{
    Debug::Assert(m_fontsAssoc.count(font) != 0, "");
		return m_fonts[m_fontsAssoc[font]];
	}

	ImFont* get(size_t id)
  {
		Debug::Assert(m_fonts.size() < id, "Out of bounds!");
    return m_fonts[id];
	}

	void set(EColor color, ImVec4 value)
  {
		m_colors.insert(std::pair(color, value));
	}

	const Colors& getColors() const { return m_colors; }
	void setColors(const Colors& colors) { m_colors = colors; }
};

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
	style.Colors[ImGuiCol_TitleBg] = IM_TAB_COLOR;
	style.Colors[ImGuiCol_TitleBgActive] = IM_TAB_COLOR;
	style.Colors[ImGuiCol_TitleBgCollapsed] = IM_TAB_COLOR;
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.38f, 0.369f, 1.00f);

	style.Colors[ImGuiCol_Tab] = IM_TAB_COLOR;
	style.Colors[ImGuiCol_TabActive] = IM_ACTIVE_COLOR;
	style.Colors[ImGuiCol_TabUnfocused] = IM_TAB_COLOR;
	style.Colors[ImGuiCol_TabUnfocusedActive] = IM_TAB_COLOR;

	style.Colors[ImGuiCol_Separator] = IM_TAB_COLOR;

	style.Colors[ImGuiCol_PlotHistogram] = IM_ACTIVE_COLOR;

	style.FramePadding.y = 8.0f;
}
