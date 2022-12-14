/**
 * \file	GUI/style.h
 * \author Martin Herich <martin.herich@phire.cz>
 *
 * Declares the tab group class
 */
#pragma once

#include "imgui.h"

#include <array>
#include <cstddef>
#include <filesystem>
#include <map>

#include "Core/Defs.h"
#include "Core/Nodes/NodeData.h"

constexpr int I3T_PROPERTY_NAME_OFFSET = 5;

enum class EColor
{
	// Global colors.
	Text,
	Border,
	WindowBackground,
	PopupBackground,
	MenuBarBackground,
	PrimaryColor, /// Color of tabs, separators, titles ...
	ActiveColor,
	TabColor, //TODO: (DR) Unused
	FloatBg,
	FloatBgActive,
	FloatBgHovered,

	DockTabActive,

	Synergies_FloatBg,
	Synergies_FloatBgActive,
	Synergies_FloatBgHovered,

	TutorialBgColor,
	TutorialText,
	TutorialBarBg,
	TutorialScrollbarGrab,
	TutorialScrollbarHovered,
	TutorialScrollbarActive,
	TutorialTitleText,
	TutorialButtonText,
	TutorialButtonBg,

	SelectionRectFull,
	SelectionRectTouch,

	// 2. Pins
	PulsePin,
	FloatPin,
	Vec3Pin,
	Vec4Pin,
	MatrixPin,
	QuatPin,
	MatrixMulPin,
	ScreenPin,

	InnerPulsePin,
	InnerFloatPin,
	InnerVec3Pin,
	InnerVec4Pin,
	InnerMatrixPin,
	InnerQuatPin,
	InnerMatrixMulPin,
	InnerScreenPin,

	Item_SelectedBorder,

	// General unspecified node
	NodeBg,
	NodeHeader,

	// Sequence
	NodeBgSequence,
	NodeHeaderSequence,

	// 3. Operator
	NodeBgOperator,
	NodeHeaderOperator,
	FloatBgOperator,
	FloatBgOperatorActive,
	FloatBgOperatorHovered,

	// 4. Transform
	NodeBgTransformation,
	NodeHeaderTranformation,
	FloatBgTransformation,
	FloatBgTransformationActive,
	FloatBgTransformationHovered,

	Nodes_Font,

	// 5. Popups
	Nodes_ConnectionPossible,
	Nodes_ConnectionNotPossible,
	Nodes_CreateNode,

	Nodes_Screen_resizeBtn_bgShape,
	Nodes_Screen_resizeBtn_bgInner,

	Nodes_Screen_resizeBtn_fgShape,
	Nodes_Screen_resizeBtn_fgInner,

	Nodes_Transformation_TrackingColor,

	Nodes_Transformation_ValidIcon_bgShape,
	Nodes_Transformation_ValidIcon_bgInner,
	Nodes_Transformation_ValidIcon_fgShape,
	Nodes_Transformation_ValidIcon_fgInner,
	Nodes_Transformation_ValidIcon_padding,

	// 6. Links
	Links_selected_colorShift,

	Builder_NodePadding, // not a color but i need a vec4
};

enum class EFont
{
	MenuLarge,
	MenuSmall,
	Button,
	Tab,
	Node,
	Header,
	TutorialText,
	TutorialTitle,
	TutorialAssignment,
	TutorialHint,
	WelcomeTitle,
	WelcomeDescription,
	WelcomeItemTitle,
	WelcomeItemDescription
};

enum class ESize
{
	Nodes_Rounding,
	Nodes_FloatWidth,
	Nodes_FloatMargin,
	Nodes_BorderWidth,
	Nodes_LabelIndent,
	Nodes_HeaderLabelIndent,
	Nodes_trackballButtonHeight,
	Nodes_TrackBallSensitivity,

	Nodes_FloatInnerPadding,

	Nodes_dragSpeedDefaulrRatio,
	Nodes_CtrlMultiplicator,
	Nodes_SHIFTMultiplicator,
	Nodes_ALTMultiplicator,

	Nodes_InputsAlignment,
	Nodes_MiddleAlignment,
	Nodes_OutputsAlignment,

	Nodes_leftSideSpacing,
	Nodes_rightSideSpacing,

	Links_ControlpointsPositionFraction,
	Links_ControlpointsPositionMin,
	Links_Thickness,
	Links_ThicknessSelectedBorder,
	Links_OffsetFraction,
	Links_OffsetMin,

	Links_selected_alpha,

	Pins_IconPadding,

	Nodes_Operators_Rounding,
	Nodes_Sequence_Rounding,

	Float_inactive_alphaMultiplicator,

	Default_VisiblePrecision,
	Default_InactiveMark
};

enum class ESizeVec2
{
	Window_FramePadding,

	Nodes_ItemsSpacing,
	Nodes_FloatPadding,
	Nodes_PinSpacing,
	Nodes_PivotAlignment,
	Nodes_PivotSize,

	Nodes_InputsSize,
	Nodes_MiddleSize,
	Nodes_OutputSize,

	Nodes_IconSize,
	Nodes_FloatCycleButtonSize,
	Nodes_ScreenTextureSize,

	Nodes_Screen_resizeButtonSize,
	Nodes_Sequence_DummySpaceSize,

	Nodes_Transformation_TrackingMarkSize,

	NewNode_positionShift,

	Builder_ItemSpacing,
};

constexpr inline EColor asColor(EValueType type) { return EColor(type); }

inline ImVec4 createColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	ImVec4 color;
	color.x = (float)r / 255.0f;
	color.y = (float)g / 255.0f;
	color.z = (float)b / 255.0f;
	color.w = (float)a / 255.0f;

	return color;
}

template <typename T> const char* enumToStr(std::map<T, const char*>& map, T en)
{
	if (!map.contains(en))
		return nullptr;
	return map[en];
}

/**
 * Global color scheme.
 */
class Theme
{
public:
	using Colors = std::unordered_map<EColor, ImVec4>;
	using Sizes = std::unordered_map<ESize, float>;
	using SizesVec = std::unordered_map<ESizeVec2, ImVec2>;

	/// \todo MH - P0919R2 Heterogeneous lookup for unordered containers, C++2a
	/// (std::unordered_map cannot be used).
	using CategoryNames = std::map<std::string, const char*>;

private:
	std::string m_name = "default";

	Colors m_colors;
	Sizes m_sizes;
	SizesVec m_sizesVec2;

	ImVec4 m_defaultColor{0.0f, 0.0f, 0.0f, 1.0f};

	std::map<EFont, size_t> m_fontsAssoc;

public:
	Theme() = default;
	Theme(std::string name, const Colors& colors, const Sizes& sizes, const SizesVec& sizesVec);

	static Theme createDefaultClassic();
	static Theme createDefaultModern();

	void initFonts();

private:
	/**
	 * Style based on Folta/Zadina style.
	 */
	void initClassicProperties();

	/**
	 * Style based on Pilka design.
	 */
	void initModernProperties();

public:
	/**
	 * Function which initializes all style properties names.
	 *
	 * \warning All values are kept in a table (key, value). If you want to edit
	 * or add new name, keep in mind, that name (value) must be unique.
	 */
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

	const ImVec2& get(ESizeVec2 sizeVec)
	{
		Debug::Assert(m_sizesVec2.contains(sizeVec), "This size is not present in the map.");
		return m_sizesVec2[sizeVec];
	}

	static const char* getCategoryName(const std::string& key);
	static CategoryNames& getCategoryNames();
	static std::map<EColor, const char*>& getColorNames();
	static std::map<ESize, const char*>& getSizeNames();
	static std::map<ESizeVec2, const char*>& getSizeVecNames();

	const std::string& getName() const { return m_name; }

	void set(EColor color, ImVec4 value) { m_colors.insert(std::pair(color, value)); }

	[[nodiscard]] const Colors& getColors() const { return m_colors; }
	Colors& getColorsRef() { return m_colors; }
	void setColors(const Colors& colors) { m_colors = colors; }

	Sizes& getSizesRef() { return m_sizes; }
	SizesVec& getSizesVecRef() { return m_sizesVec2; }

	// JH unused -> maybe for "InputItems" only - drag float etc... but probably
	// not needed
	//	void operatorColorTheme();
	//	void transformationColorTheme();
	void returnFloatColorToDefault();

	// JH unused
	//	ImVec4 getHeader();
	//	ImVec4 getBg();

private:
	template <typename E, typename T>
	void copyProperties(std::unordered_map<E, T>& target, const std::unordered_map<E, T>& source)
	{
		for (const auto [key, val] : source)
			target[key] = val;
	}
};
