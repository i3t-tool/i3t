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
#include "Core/Nodes/NodeData.h"

enum class EColor
{
	Text,
	Border,
	WindowBackground,
	PopupBackground,
	MenuBarBackground,
	PrimaryColor, /// Color of tabs, separators, titles ...
	ActiveColor,
	TabColor,

	TutorialBgColor,

	PulseLink,
	FloatLink,
	MatrixLink,
	QuatLink,
	Vec3Link,
	Vec4Link,
	ScreenLink
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
	Header,
	I3TTitle,
	I3TDescription,
	IntroItemTitle,
	IntroItemDescription
};

enum class ESize
{
	Nodes_Rounding,
	Nodes_FloatWidth,
	Nodes_FloatMargin,
	Nodes_ItemsSpacingX,
	Nodes_ItemsSpacingY,
	Window_FramePadding,
	COUNT
};

constexpr inline EColor asColor(EValueType type)
{
	switch (type)
	{
	case EValueType::Pulse:
		return EColor::PulseLink;
	case EValueType::Float:
		return EColor::FloatLink;
	case EValueType::Vec3:
		return EColor::Vec3Link;
	case EValueType::Vec4:
		return EColor::Vec4Link;
	case EValueType::Matrix:
		return EColor::MatrixLink;
	case EValueType::Quat:
		return EColor::QuatLink;
	case EValueType::MatrixMul:
		return EColor::MatrixLink;
	case EValueType::Screen:
		return EColor::ScreenLink;
	}

	return EColor::Vec3Link;
}

/**
 * Global color scheme.
 */
class Theme
{
	using Colors = std::map<EColor, ImVec4>;
	Colors m_colors;

	ImVec4 m_defaultColor{0.0f, 0.0f, 0.0f, 1.0f};

	/// \todo MH Set dynamic scale (reload font in runtime).
	static constexpr float m_fontScale = 1.2f;
	std::map<EFont, size_t> m_fontsAssoc;
	std::vector<ImFont*> m_fonts;
	std::array<float, static_cast<size_t>(ESize::COUNT)> m_sizes;

public:
	/**
	 * Creates default global color scheme based on Lukas Pilka's design.
	 */
	Theme();
	Theme(const Colors& colors) { m_colors = colors; }

	void init();

	/**
	 * Call this function whenever you change style settings.
	 */
	void apply();
	const ImVec4& get(EColor color)
	{
		if (m_colors.count(color) == 0)
			return m_defaultColor;

		return m_colors[color];
	}

	ImFont* get(EFont font)
	{
		Debug::Assert(m_fontsAssoc.count(font) != 0, "");
		return m_fonts[m_fontsAssoc[font]];
	}

	ImFont* get(size_t id)
	{
		Debug::Assert(id < m_fonts.size(), "Out of bounds!");
		return m_fonts[id];
	}

	float get(ESize size)
	{
		Debug::Assert(size != ESize::COUNT, "Strange size, isn't it?");
		return m_sizes[static_cast<size_t>(size)];
	}

	void set(EColor color, ImVec4 value) { m_colors.insert(std::pair(color, value)); }

	const Colors& getColors() const { return m_colors; }
	void setColors(const Colors& colors) { m_colors = colors; }
};
