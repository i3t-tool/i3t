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
	 * Creates default global color scheme based on Zadina's thesis.
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
