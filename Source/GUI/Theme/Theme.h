/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "imgui.h"

#include <array>
#include <cstddef>
#include <filesystem>
#include <map>

#include "Core/Defs.h"
#include "Core/Nodes/NodeData.h"
#include "Core/Result.h"
#include "GUI/ThemeVariable.h"

constexpr int I3T_PROPERTY_NAME_OFFSET = 5;

constexpr auto I3T_CLASSIC_THEME_NAME = "Classic";
constexpr auto I3T_DEFAULT_THEME_LIGHT_NAME = "LightMode";
constexpr auto I3T_DEFAULT_THEME_DARK_NAME = "DarkMode";

enum class EColor
{
	// Main I3T colors
	PrimaryColor,   /// Color of tabs, separators, titles ...
	ActiveColor,    /// Focused primary color
	SelectionColor, /// Active selection color

	// ImGui standard colors
	Text,
	TextDisabled,
	WindowBackground,
	PopupBackground,
	Border,
	FrameBg,
	FrameBgHovered,
	FrameBgActive,
	MenuBarBackground,
	Button,
	ButtonHovered,
	ButtonActive,
	Tab,
	TabHovered,
	TabActive,

	// Extra dock tab options
	DockTab,
	DockTabActive,
	DockTabUnfocused,
	DockTabUnfocusedActive,
	DockTabHovered,

	FloatBg,
	FloatBgActive,
	FloatBgHovered,
	SceneViewBackground,

	Synergies_FloatBg,
	Synergies_FloatBgActive,
	Synergies_FloatBgHovered,

	Workspace_SelectedBorder,
	Workspace_FocusBorder,

	TutorialBgColor,
	TutorialText,
	TutorialBarBg,
	TutorialScrollbarBg,
	TutorialScrollbarGrab,
	TutorialScrollbarHovered,
	TutorialScrollbarActive,
	TutorialTitleText,
	TutorialHighlightText,
	TutorialButtonText,
	TutorialButtonBg,
	TutorialButtonActive,
	TutorialButtonHovered,
	TutorialTaskBg,

	SelectionRectFull,
	SelectionRectTouch,

	TrackingSequenceTint,

	AddMenuHeader,

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

	// General unspecified node
	NodeBg,
	NodeHeader,
	NodeFont,

	// 3. Operator
	NodeBgOperator,
	NodeHeaderOperator,
	FloatBgOperator,
	FloatBgOperatorActive,
	FloatBgOperatorHovered,

	// 4. Transform
	NodeBgTransformation,
	NodeBorder,
	NodeHeaderTranformation,
	NodeLODButtonColorText,
	NodeContextButtonColorText,
	NodeLODButtonColor,
	NodeLODButtonColorActive,
	NodeLODButtonColorHovered,
	FloatBgTransformation,
	FloatBgTransformationActive,
	FloatBgTransformationHovered,

	// 5. Popups
	Nodes_ConnectionPossible,
	Nodes_ConnectionNotPossible,
	Nodes_CreateNode,

	Nodes_Screen_resizeBtn_bgShape,
	Nodes_Screen_resizeBtn_bgInner,

	Nodes_Screen_resizeBtn_fgShape,
	Nodes_Screen_resizeBtn_fgInner,

	Nodes_Screen_noInput_background,
	Nodes_Screen_noInput_text,

	Nodes_Transformation_TrackingMarkColor,
	Nodes_Transformation_TrackingColor,

	Nodes_Transformation_ValidIcon_bgShape,
	Nodes_Transformation_ValidIcon_bgInner,
	Nodes_Transformation_ValidIcon_fgShape,
	Nodes_Transformation_ValidIcon_fgInner,
	Nodes_Transformation_ValidIcon_padding,

	// 6. Links
	Links_selected_colorShift,

	Builder_NodePadding, // not a color but i need a vec4

	StartWindow_DescriptionBackground,
	StartWindow_WindowBackground,
	StartWindow_ScrollbarBackground,
	StartWindow_ScrollbarGrab,
	StartWindow_ScrollbarGrabHovered,
	StartWindow_ScrollbarGrabActive,
	StartWindow_Separator,
	StartWindow_DefaultButton,
	StartWindow_NewSceneButton,
	StartWindow_NewSceneButtonFont,
	StartWindow_TitleFont,
	StartWindow_DescriptionFont,
	StartWindow_YourSceneWinBackground,

	AboutWindow_BackgroundLeft,
	AboutWindow_BackgroundRight,
	AboutWindow_Text,

	Cycle_Button,
	Cycle_ButtonHovered,
	Cycle_ButtonActive,
	Cycle_ButtonForeground, ////< large buttons icons and text on radiobutton
	Cycle_RadioButton,
	Cycle_RadioButtonActive,
	Cycle_RadioButtonHovered,
	Cycle_RadioButtonText,
	Cycle_RadioButtonSelected,
	Cycle_RadioButtonSelectedHovered,
	Cycle_RadioButtonSelectedText,
	Cycle_RadioButtonBackground,
};

enum class EFont
{
	Mono,
	Regular,
	Button,

	TutorialText,
	TutorialTitle,
	TutorialBold,
	TutorialHint,
	WelcomeTitle,
	WelcomeItemTitle
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

	Workspace_SelectedBorderThickness,
	Workspace_FocusBorderThickness,
	Workspace_CopyPasteOffset,
	Workspace_TrackingTimeBetweenTracks,

	TutorialTaskSquareXPadding,
	TutorialWindow_FrameRounding,
	TutorialWindow_ScrollbarSize,
	TutorialWindow_ScrollbarRounding,
	TutorialWindow_BackButtonWidth,
	TutorialWindow_MainMenuButtonWidth,

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
	Nodes_LOD_Button_Rounding,
	Nodes_Border_Rounding,
	Nodes_Border_Thickness,

	Nodes_Transformation_TrackingMarkSize,

	Float_inactive_alphaMultiplicator,

	Default_VisiblePrecision,
	Default_VisibleQuaternionPrecision,
	Default_InactiveMark, // Unused

	Tracking_SmoothScrollSpeed,
	Tracking_JaggedScrollSpeed,

	Tooltip_Rounding,

	Window_Rounding,

	StartWindow_WinWidth,
	StartWindow_WinHeight,
	StartWindow_WinRounding,
	StartWindow_TitleVerticalOffset,
	StartWindow_LeftBarWidth,
	StartWindow_LoadButtonWidth,
	StartWindow_StartNewButtonWidth,
	StartWindow_ButtonHeight,
	StartWindow_ThumbImageSize,
	StartWindow_StartButtonWidth,
	StartWindow_FrameRounding,
	StartWindow_ScrollbarSize,
	StartWindow_YourSceneWinRounding,
	StartWindow_DotSize,
	StartWindow_DotSpacing,

	Cycle_ButtonRounding,
	Cycle_RadioButtonRounding,
};

enum class ESizeVec2
{
	Window_FramePadding,
	Window_Padding,

	TutorialWindow_Padding,

	Tooltip_Padding,

	Nodes_ItemsSpacing,
	Nodes_FloatPadding,   // Maybe rename to frame padding?
	Nodes_PinSpacing,     // Unused
	Nodes_PivotAlignment, // Unused
	Nodes_PivotSize,      // Unused

	Nodes_InputsSize,
	Nodes_MiddleSize,
	Nodes_OutputSize,

	Nodes_LODButtonSize,

	Nodes_PinSize,
	Nodes_PinSize_MatrixMul,
	Nodes_FloatCycleButtonSize,
	Nodes_ScreenTextureSize,

	Nodes_Screen_resizeButtonSize,

	Nodes_Sequence_DummySpaceSize, // TODO: Hook up to drop indicator, create another for empty sequence dummy size

	Nodes_noPinsSpacing,

	NewNode_positionShift,

	Builder_ItemSpacing,

	StartWindow_WinSize,
	StartWindow_LogoOffset,
	StartWindow_WinPadding,
	StartWindow_LeftWinPadding,
	StartWindow_RightWinOuterPadding,
	StartWindow_RightWinInnerPadding,

	Cycle_ButtonSize,
};

inline ImVec4 createColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	ImVec4 color;
	color.x = (float) r / 255.0f;
	color.y = (float) g / 255.0f;
	color.z = (float) b / 255.0f;
	color.w = (float) a / 255.0f;

	return color;
}

template <typename T>
const char* enumToStr(const std::map<T, const char*>& map, T en)
{
	if (!map.contains(en))
	{
		return nullptr;
	}
	return map.at(en);
}

/**
 * Application Theme holding custom style variables that override the current ImGui style and/or are used separately by
 * various parts of the I3T code.
 * The Theme holds color information as well as general sizing information.
 *
 * Themes do have a UI/DPI scaling factor using which relevant sizing variables are scaled when fetched.
 * Changes of the scaling factor are however handled elsewhere (in UIModule).
 *
 * Themes do hold an association between font keys and actual font names.
 * But fonts are loaded and managed externally (by the FontManager).
 * Currently the Theme cannot load new fonts, but it can change which fonts are used where.
 * // Proposal: (Issue #432) Themes could tell FontManager which fonts to load
 */
class Theme
{
public:
	using Colors = std::unordered_map<EColor, ImVec4>;
	/// Map of ESizeVec2 and a pair of float and bool, bool specifies whether the float is dpi scaled.
	using Sizes = std::unordered_map<ESize, std::pair<float, bool>>;
	/// Map of ESizeVec2 and a pair of ImVec2 and bool, bool specifies whether the ImVec2 is dpi scaled.
	using SizesVec = std::unordered_map<ESizeVec2, std::pair<ImVec2, bool>>;

	/// \todo MH - P0919R2 Heterogeneous lookup for unordered containers, C++2a
	/// (std::unordered_map cannot be used).
	using CategoryNames = std::map<std::string, const char*>;

public:
	Theme() = default;
	Theme(std::string name, bool isDark, const Colors& colors, const Sizes& sizes, const SizesVec& sizesVec);

	static Theme createDefaultClassic();

	void initFonts();

private:
	/**
	 * Style based on Folta/Zadina style.
	 */
	void initClassicProperties();

public:
	/**
	 * Function which initializes all style properties names.
	 *
	 * \warning All values are kept in a table (key, value). If you want to edit
	 * or add new name, keep in mind, that name (value) must be unique.
	 */
	static void initNames();

	/**
	 * Initializes ImGuiStyle with default values. Rewrites all ImGuiStyle values.
	 * The theme still needs to be applied after this call.
	 */
	static void initImGuiStyle();

	void updateDiwneStyleFromTheme() const;

	/**
	 * Call this function whenever you change style settings.
	 * Applies Theme settings to the current ImGuiStyle.
	 */
	void apply();

	const ImVec4& get(EColor color)
	{
		if (m_colors.count(color) == 0)
			return m_defaultColor;

		return m_colors[color];
	}

	ImFont* get(EFont font);

	float get(ESize size)
	{
		const auto entry = m_sizes[size];
		return entry.second ? entry.first * m_dpiScale : entry.first;
	}

	ImVec2 get(ESizeVec2 sizeVec)
	{
		I3T_ASSERT(m_sizesVec2.contains(sizeVec), "This size is not present in the map.");
		const auto entry = m_sizesVec2[sizeVec];
		return entry.second ? entry.first * m_dpiScale : entry.first;
	}

	static const char* getCategoryName(const std::string& key);
	static CategoryNames& getCategoryNames();
	static std::map<EColor, const char*>& getColorNames();
	static std::map<ESize, const char*>& getSizeNames();
	static std::map<ESizeVec2, const char*>& getSizeVecNames();

	const std::string& getName() const
	{
		return m_name;
	}

	void setDpiScale(float scale)
	{
		m_dpiScale = scale;
	}
	float getDpiScale() const
	{
		return m_dpiScale;
	}

	const bool isDark() const
	{
		return m_isDark;
	}

	void set(EColor color, ImVec4 value)
	{
		m_colors.insert(std::pair(color, value));
	}

	Colors& getColorsRef()
	{
		return m_colors;
	}

	Sizes& getSizesRef()
	{
		return m_sizes;
	}

	SizesVec& getSizesVecRef()
	{
		return m_sizesVec2;
	}

	static const std::vector<ThemeGroup>& getVariables()
	{
		return s_variables;
	}

private:
	template <typename E, typename T>
	void copyColors(std::unordered_map<E, T>& target, const std::unordered_map<E, T>& source)
	{
		for (const auto [key, val] : source)
			target[key] = val;
	}

	template <typename E, typename T>
	void copySizes(std::unordered_map<E, T>& target, const std::unordered_map<E, T>& source)
	{
		for (const auto [key, val] : source)
		{
			auto it = target.find(key);
			if (it != target.end())
			{
				it->second = {val.first, it->second.second};
			}
			else
			{
				target.emplace(key, val);
			}
		}
	}

	static ThemeGroup& group(const char* name);

	std::string m_name = "default";

	float m_dpiScale = 1.0f;
	bool m_isDark = true;
	Colors m_colors;
	Sizes m_sizes;
	SizesVec m_sizesVec2;

	ImVec4 m_defaultColor{0.0f, 0.0f, 0.0f, 1.0f};

	std::map<EFont, std::string> m_fontsAssoc;

	static inline std::vector<ThemeGroup> s_variables;
};

namespace Detail
{
bool isLightThemeSet();
}
