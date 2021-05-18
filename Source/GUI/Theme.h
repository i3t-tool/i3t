/**
 * \file	GUI/style.h
 *
 * Declares the tab group class
 */
#pragma once

#include "imgui.h"

#include <array>
#include <map>
#include <optional>

#include "Core/Defs.h"
#include "Core/Nodes/NodeData.h"

enum class EColor
{
	PulseLink = 0,
	FloatLink,
	Vec3Link,
	Vec4Link,
	MatrixLink,
	QuatLink,
	MatrixMulLink,
	ScreenLink,

	Text,
	Border,
	WindowBackground,
	PopupBackground,
	MenuBarBackground,
	PrimaryColor, /// Color of tabs, separators, titles ...
	ActiveColor,
	TabColor,
  FloatBg,

	NodeBgOperator,
	NodeHeaderOperator,
	FloatBgOperator,
	FloatBgOperatorActive,
	FloatBgOperatorHovered,

	NodeBgTransformation,
	NodeHeaderTranformation,
	FloatBgTransformation,
	FloatBgTransformationActive,
	FloatBgTransformationHovered,

	NodeHeader,
	NodeEditorBg,
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
	TitleSmall,
};

enum class ESize
{
	Nodes_Rounding,
	Nodes_FloatWidth,
	Nodes_FloatMargin,
	Nodes_BorderWidth,
	Nodes_LabelIndent,

	COUNT
};

enum class ESizeVec2
{
	Window_FramePadding,

	Nodes_ItemsSpacing,
	Nodes_FloatPadding,
	Nodes_PinSpacing,
	Nodes_PivotAlignment,
	Nodes_IconSize,
};

constexpr inline EColor asColor(EValueType type)
{
	return EColor(type);
}



template <typename T>
std::optional<const char*> enumToStr(std::map<T, const char*>& map, T en)
{
	if (!map.contains(en))
		return std::nullopt;
	return map[en];
}

/**
 * Global color scheme.
 */
class Theme
{
	friend Theme createDefaultTheme();

public:
	using Colors = std::unordered_map<EColor, ImVec4>;
	using Sizes = std::unordered_map<ESize, float>;
	using SizesVec = std::unordered_map<ESizeVec2, ImVec2>;

private:
	std::string m_name = "default";

	Colors m_colors;
	Sizes m_sizes;
	SizesVec m_sizesVec2;

	ImVec4 m_defaultColor{0.0f, 0.0f, 0.0f, 1.0f};

	std::map<EFont, size_t> m_fontsAssoc;

public:
	/**
	 * Creates default global color scheme based on Lukas Pilka's design.
	 */
	Theme();
	Theme(std::string name, const Colors& colors, const Sizes& sizes, const SizesVec& sizesVec);

	static Theme createDefault();

	void initFonts();
	void initDefaultProperties();

	static void initNames();

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

	ImFont* get(EFont font);

	float get(ESize size) { return m_sizes[size]; }

	ImVec2& get(ESizeVec2 sizeVec)
	{
		Debug::Assert(m_sizesVec2.contains(sizeVec), "This size is not present in the map.");
		return m_sizesVec2[sizeVec];
	}

	static std::map<EColor, const char*>& getColorNames();
	static std::map<ESize, const char*>& getSizeNames();
	static std::map<ESizeVec2, const char*>& getSizeVecNames();

	const std::string& getName() { return m_name; }

	void set(EColor color, ImVec4 value) { m_colors.insert(std::pair(color, value)); }

	[[nodiscard]] const Colors& getColors() const { return m_colors; }
	Colors& getColorsRef() { return m_colors; }
	void setColors(const Colors& colors) { m_colors = colors; }

	Sizes& getSizesRef() { return m_sizes; }
	SizesVec& getSizesVecRef() { return m_sizesVec2; }

	void operatorColorTheme();
	void transformationColorTheme();
  void returnFloatColorToDefault();

	ImVec4 getHeader();

private:
	template <typename E, typename T>
	void copyProperties(std::unordered_map<E, T>& target, const std::unordered_map<E, T>& source)
	{
		for (const auto [key, val] : source)
			target[key] = val;
	}
};
