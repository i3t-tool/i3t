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

#include <cstddef>
#include <filesystem>
#include <map>

#include "DIWNE/Core/Style/StyleOverride.h"
#include "ThemeVariable.h"

constexpr auto I3T_CLASSIC_THEME_NAME = "Classic";
constexpr auto I3T_DEFAULT_THEME_LIGHT_NAME = "LightMode";
constexpr auto I3T_DEFAULT_THEME_DARK_NAME = "DarkMode";

//
// To create a new theme style variable:
// 1. Define an enum key in one of style enums EColor/ESize/ESizeVec2 ...
// 2. Register the style variable name in Theme::initNames(), add it to one of the groups and define its name
// 3. Set its default value in Theme::initDefaultClassic()
//

enum class EColor
{
	// TODO: (DR) Many important ImGui style colors are not covered by the theme keys!
	//  Hence it's impossible to change color of buttons, menu items, resize handles, checkboxes etc. etc!
	// ImGui standard colors, in the order of definition of ImGuiCol_ in imgui.h
	Text,
	TextDisabled,
	TextDark, // Non-standard, used for "black" text and text shadows
	WindowBackground,
	ChildBackground,
	PopupBackground,
	Border,
	BorderShadow,
	BorderDim,
	FrameBg,
	FrameBgHovered,
	FrameBgActive,
	WindowTitleBg,
	WindowTitleBgActive,
	WindowTitleBgCollapsed,
	MenuBarBackground,
	ScrollbarBg,
	ScrollbarGrab,
	ScrollbarGrabHovered,
	ScrollbarGrabActive,
	CheckMark,
	SliderGrab,
	SliderGrabActive,
	Button,
	ButtonHovered,
	ButtonActive,
	Header,
	HeaderHovered,
	HeaderActive,
	Separator,
	SeparatorHovered,
	SeparatorActive,
	ResizeGrip,
	ResizeGripHovered,
	ResizeGripActive,
	Tab,
	TabHovered,
	TabActive,
	TabUnfocused,
	TabUnfocusedActive,
	DockingPreview,
	DockingEmptyBg,
	PlotLines,
	PlotLinesHovered,
	PlotHistogram,
	PlotHistogramHovered,
	TableHeaderBg,
	TableBorderStrong,
	TableBorderLight,
	TableRowBg,
	TableRowBgAlt,
	TextSelectedBg,
	DragDropTarget,
	NavHighlight,
	NavWindowingHighlight,
	NavWindowingDimBg,
	ModalWindowDimBg,

	// Extra dock tab options
	DockTab,
	DockTabActive,
	DockTabUnfocused,
	DockTabUnfocusedActive,
	DockTabHovered,

	// Custom general stuff
	ButtonDim,
	ButtonDimHovered,
	ButtonDimActive,
	ButtonDimToggled,
	ButtonDimDark,

	FloatBg,
	FloatBgActive,
	FloatBgHovered,

	Synergies_FloatBg,
	Synergies_FloatBgActive,
	Synergies_FloatBgHovered,

	Workspace_SelectedBorder,
	Workspace_HoverBorder,

	// TODO: (DR) Tutorials should try to reuse colors from the more general theme colors
	//  its extra keys to maintain, they are hence harder to change and are inconsistent with the rest of the app
	//	Point is they are separated for no good reason, because they were developed separately
	//  Same goes for the Start Window and some of the extra node editor styling stuff (cycle)
	//  We should strive to reduce the total amount of keys if possible

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

	// Scene View (3D Viewport)
	SceneViewBackground,
	SceneViewBackgroundLocalColorShift,
	SceneViewBackgroundTrackingColorShift,

	SceneViewGridColor,
	SceneViewGridX,
	SceneViewGridY,
	SceneViewGridZ,
	SceneViewGridLocalColor,
	SceneViewGridLocalX,
	SceneViewGridLocalY,
	SceneViewGridLocalZ,

	// Node Editor
	NodeEditorBackground,
	NodeEditorGridColor,
	NodeEditorGridDotsColor,
	NodeDropZoneBackground,

	// Pins
	DisabledPinColor,
	PinHoverColorShift,
	PinHoverBackground,
	PinSocketBackground,
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
	NodeFg,
	NodeHeaderFg,
	NodeFont,

	// 3. Operator
	NodeBgOperator,
	NodeHeaderOperator,

	// 4. Transform
	NodeBgTransformation,
	NodeBorder,
	NodeHeaderTranformation,
	NodeLODButtonColorText,
	NodeContextButtonColorText,
	NodeLODButtonColor,
	NodeLODButtonColorActive,
	NodeLODButtonColorHovered,

	// 5. Popups
	Nodes_ConnectionPossible,
	Nodes_ConnectionNotPossible,
	Nodes_CreateNode,

	Nodes_Screen_resizeBtn_bgShape,
	Nodes_Screen_resizeBtn_bgInner,

	Nodes_Screen_resizeBtn_fgShape,
	Nodes_Screen_resizeBtn_fgInner,

	Nodes_Screen_background,
	Nodes_Screen_noInput_text,

	Nodes_Tracking_ColorActive,
	Nodes_Tracking_ColorInactive,
	Nodes_Tracking_Cursor,
	Nodes_Tracking_CursorHovered,
	Nodes_Tracking_CursorActive,
	Nodes_Tracking_OverlayActive,
	Nodes_Tracking_OverlayInactive,

	Nodes_Transformation_ValidIcon_bgShape,
	Nodes_Transformation_ValidIcon_bgInner,
	Nodes_Transformation_ValidIcon_fgShape,
	Nodes_Transformation_ValidIcon_fgInner,
	Nodes_Transformation_ValidIcon_padding,

	// 6. Links
	Links_selected_colorShift, // Unused

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
	Tooltip_Rounding,
	Window_Rounding,
	Frame_Rounding,

	FloatingButtonRounding,

	MouseDragThreshold,

	Nodes_Rounding,
	Nodes_BorderWidth,           // Unused // TODO: Impl
	Nodes_LabelIndent,           // Unused // TODO: What does this mean?
	Nodes_HeaderLabelIndent,     // Unused // TODO: Impl
	Nodes_trackballButtonHeight, // TODO: [Trackball]
	Nodes_TrackBallSensitivity,  // TODO: [Trackball]

	Nodes_FloatInnerPadding,
	Nodes_FloatCharacterWidthMultiplier,
	Nodes_FloatMinCharacters,
	Float_inactive_alphaMultiplicator,

	Nodes_dragSpeedDefaultRatio,
	Nodes_CtrlMultiplicator,  // Unused
	Nodes_SHIFTMultiplicator, // Unused
	Nodes_ALTMultiplicator,   // Unused

	Nodes_InputsAlignment,  // Unused
	Nodes_MiddleAlignment,  // Unused
	Nodes_OutputsAlignment, // Unused

	Nodes_leftSideSpacing,  // Unused
	Nodes_rightSideSpacing, // Unused

	Workspace_SelectedBorderThickness,
	Workspace_HoverBorderThickness,

	Tutorial_TaskSquareXPadding,
	Tutorial_FrameRounding,
	Tutorial_ScrollbarSize,
	Tutorial_ScrollbarRounding,
	Tutorial_BackButtonWidth,
	Tutorial_MainMenuButtonWidth,

	Links_ControlpointsPositionFraction,
	Links_ControlpointsPositionMin,
	Links_ControlpointsPositionMax,
	Links_Thickness,
	Links_ThicknessSelected,
	Links_selected_alpha,

	Pins_IconPadding,
	Pins_DragAssistRadius,

	Nodes_Operators_Rounding,
	Nodes_Sequence_Rounding,
	Nodes_LOD_Button_Rounding,
	Nodes_Border_Rounding,
	Nodes_Border_Thickness,

	Nodes_Tracking_CursorSize,
	Nodes_Tracking_LinkWidth,

	Default_VisiblePrecision,
	Default_VisibleQuaternionPrecision,
	Default_InactiveMark, // Unused

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
	FramePadding,
	Window_Padding,

	Tutorial_WindowPadding,

	Tooltip_Padding,

	Nodes_ItemsSpacing,
	Nodes_FloatPadding, // Maybe rename to frame padding?

	Nodes_LODButtonSize,

	Nodes_PinSize,
	Nodes_PinSize_MatrixMul,
	Nodes_PinSquareHoverEnlarge,

	Nodes_FloatCycleButtonSize,
	Nodes_ScreenTextureSize,

	Nodes_Screen_resizeButtonSize,

	Nodes_Sequence_DummySpaceSize, // TODO: Hook up to drop indicator, create another for empty sequence dummy size

	// Nodes_noPinsSpacing,

	Nodes_NewPositionShift,

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
const char* enumToStr(const std::map<T, std::string>& map, T en)
{
	if (!map.contains(en))
	{
		return nullptr;
	}
	return map.at(en).c_str();
}

template <typename T>
std::optional<T> strToEnum(std::map<T, std::string>& map, std::string&& name)
{
	for (const auto& [key, val] : map)
	{
		if (val == name)
		{
			return key;
		}
	}
	return std::nullopt;
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
	// TODO: (DR) Same like with g_names, why are these maps? Enums are unique int ids we can just use a vector.
	using Colors = std::unordered_map<EColor, ImVec4>;
	/// Map of ESizeVec2 and a pair of float and bool, bool specifies whether the float is dpi scaled.
	using Sizes = std::unordered_map<ESize, std::pair<float, bool>>;
	/// Map of ESizeVec2 and a pair of ImVec2 and bool, bool specifies whether the ImVec2 is dpi scaled.
	using SizesVec = std::unordered_map<ESizeVec2, std::pair<ImVec2, bool>>;

	// DIWNE node styles
	static DIWNE::StyleOverride m_nodeStyle;
	static DIWNE::StyleOverride m_transformationStyle;
	static DIWNE::StyleOverride m_operatorStyle;

	Theme() = default;
	Theme(std::string name, bool isDark, const Colors& colors, const Sizes& sizes, const SizesVec& sizesVec);

	static Theme createDefaultClassic();

	void initFonts();

private:
	/**
	 * Style based on Folta/Zadina style.
	 */
	void initDefaultClassic();

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

	/// Returns color variable value
	const ImVec4& get(EColor color)
	{
		if (m_colors.count(color) == 0)
			return m_defaultColor;

		return m_colors[color];
	}
	/// Returns a color whose alpha is set to the provided valuea
	ImVec4 get(EColor color, float alpha)
	{
		const ImVec4& col = get(color);
		return ImVec4(col.x, col.y, col.z, alpha);
	}
	/// Returns a pointer to a color variable
	ImVec4* getPtr(EColor color)
	{
		if (m_colors.count(color) == 0)
			return &m_defaultColor;

		return &m_colors[color];
	}

	ImFont* get(EFont font);

	/// Returns size variable value, scaled by DPI factor.
	float get(ESize size)
	{
		const auto entry = m_sizes[size];
		return entry.second ? entry.first * m_dpiScale : entry.first;
	}
	/// Returns a pointer to a size variable. Not DPI scaled.
	float* getPtr(ESize size)
	{
		auto& entry = m_sizes[size];
		return &entry.first;
	}

	/// Returns 2D size variable value, scaled by DPI factor.
	ImVec2 get(ESizeVec2 sizeVec)
	{
		const auto& entry = m_sizesVec2[sizeVec];
		return entry.second ? entry.first * m_dpiScale : entry.first;
	}
	/// Returns a pointer to a 2D size variable. Not DPI scaled.
	ImVec2* getPtr(ESizeVec2 sizeVec)
	{
		auto& entry = m_sizesVec2[sizeVec];
		return &entry.first;
	}

	static std::map<EColor, std::string>& getColorNames();
	static std::map<ESize, std::string>& getSizeNames();
	static std::map<ESizeVec2, std::string>& getSizeVecNames();

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

	float getBorderSize() const
	{
		return std::max(1.0f, IM_TRUNC(m_dpiScale));
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

	static ThemeGroup& group(const char* name, const char* id, int indent = 0);

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
