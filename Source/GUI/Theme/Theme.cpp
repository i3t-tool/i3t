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
#include "Theme.h"

// #include "imgui_node_editor.h"

#include "Config.h"
#include "I3T.h"
#include "Utils/HSLColor.h"

static Theme::CategoryNames g_CategoryNames;
static std::map<EColor, const char*> g_ColorNames;
static std::map<ESize, const char*> g_SizeNames;
static std::map<ESizeVec2, const char*> g_SizeVecNames;

template <typename T>
concept Enumerable = std::is_enum_v<T>;

template <Enumerable T>
static void checkEnumProperties(const std::set<T> values)
{
	const auto size = magic_enum::enum_count<T>();
	for (int i = 0; i < size; ++i)
	{
		const auto enumValue = T(i);
		if (!values.contains(enumValue))
		{
			LOG_DEBUG("Description for {}::{} is missing.", magic_enum::enum_type_name<T>(),
			          magic_enum::enum_name(enumValue));
		}
	}
}

static void checkThemeVariables()
{
	LOG_DEBUG("Checking for missing theme variables descriptions");

	std::set<EColor> colors;
	std::set<ESize> sizes;
	std::set<ESizeVec2> sizeVecs;

	for (const auto& group : Theme::getVariables())
	{
		for (const auto& var : group.variables)
		{
			std::visit(Detail::Overloaded{[&](EColor color) {
				                              colors.insert(color);
			                              },
			                              [&](ESize size) {
				                              sizes.insert(size);
			                              },
			                              [&](ESizeVec2 sizeVec) {
				                              sizeVecs.insert(sizeVec);
			                              }},
			           var.key);
		}
	}

	checkEnumProperties(colors);
	checkEnumProperties(sizes);
	checkEnumProperties(sizeVecs);
}

Theme Theme::createDefaultClassic()
{
	Theme theme;

	theme.initClassicProperties();
	theme.initFonts();

	theme.m_name = I3T_CLASSIC_THEME_NAME;

	return theme;
}

Theme::Theme(std::string name, bool isDark, const Theme::Colors& colors, const Theme::Sizes& sizes,
             const Theme::SizesVec& sizesVec)
{
	initClassicProperties();

	m_name = std::move(name);
	m_isDark = isDark;
	copyProperties(m_colors, colors);
	copyProperties(m_sizes, sizes);
	copyProperties(m_sizesVec2, sizesVec);
}

void Theme::initFonts()
{
	auto& io = ImGui::GetIO();

	m_fontsAssoc.insert(std::pair(EFont::Regular, "Roboto14"));
	m_fontsAssoc.insert(std::pair(EFont::LargeBold, "RobotoBold20"));

	m_fontsAssoc.insert({EFont::Mono, "RobotoMono14"});
	m_fontsAssoc.insert(std::pair(EFont::MenuLarge, "Roboto14"));
	m_fontsAssoc.insert(std::pair(EFont::TutorialText, "Roboto14"));
	m_fontsAssoc.insert(std::pair(EFont::Button, "RobotoBold12"));
	m_fontsAssoc.insert(std::pair(EFont::Tab, "RobotoBold12"));
	m_fontsAssoc.insert(std::pair(EFont::Node, "Roboto12"));
	m_fontsAssoc.insert(std::pair(EFont::MenuSmall, "Roboto12"));
	m_fontsAssoc.insert(std::pair(EFont::TutorialTitle, "UbuntuBold24"));

	io.FontDefault = get(EFont::Regular);
}

void Theme::initClassicProperties()
{
	set(EColor::Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	set(EColor::TextDisabled, ImVec4(0.50f, 0.50f, 0.50f, 1.0f));
	set(EColor::WindowBackground, ImVec4(0.439f, 0.439f, 0.455f, 1.00f));
	set(EColor::PopupBackground, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	set(EColor::Border, ImVec4(0.278f, 0.278f, 0.286f, 1.00f));
	set(EColor::FrameBg, ImVec4(0.16f, 0.29f, 0.48f, 0.54f));
	set(EColor::FrameBgHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	set(EColor::FrameBgActive, ImVec4(0.26f, 0.59f, 0.98f, 0.67f));
	set(EColor::MenuBarBackground, ImVec4(0.40f, 0.38f, 0.369f, 1.00f));
	set(EColor::Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	set(EColor::ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.00f));
	set(EColor::ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.00f));
	set(EColor::Tab, ImVec4(0.18f, 0.35f, 0.58f, 0.86f));
	set(EColor::TabHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.00f));
	set(EColor::TabActive, ImVec4(0.20f, 0.41f, 0.68f, 1.00f));

	set(EColor::PrimaryColor, ImVec4(0.255f, 0.255f, 0.251f, 1.00f));
	set(EColor::ActiveColor, ImVec4(0.278f, 0.278f, 0.286f, 1.00f));
	set(EColor::SelectionColor, ImVec4(0.259f, 0.588f, 0.980f, 1.00f));

	set(EColor::FloatBg, ImVec4(0.16f, 0.29f, 0.48f, 0.54f));
	set(EColor::FloatBgHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.4f));
	set(EColor::FloatBgActive, ImVec4(0.26f, 0.59f, 0.98f, 0.67f));
	set(EColor::AddMenuHeader, ImVec4(0.5f, 0.5f, 0.5f, 1.f));
	set(EColor::SceneViewBackground, ImVec4(0.3f, 0.3f, 0.35f, 1.f));

	set(EColor::DockTab, ImVec4(0.309f, 0.309f, 0.318f, 1.f));
	set(EColor::DockTabActive, ImVec4(0.258f, 0.334f, 0.427f, 1.f));
	set(EColor::DockTabUnfocused, get(EColor::ActiveColor));
	set(EColor::DockTabUnfocusedActive, ImVec4(0.263f, 0.291f, 0.325f, 1.f));
	set(EColor::DockTabHovered, get(EColor::SelectionColor));

	set(EColor::Workspace_SelectedBorder, createColor(88, 255, 234, 150));    // TODO: Missing in DarkTheme
	set(EColor::Workspace_FocusBorder, createColor(0, 0, 0, 50));             // TODO: Missing name!
	set(EColor::Workspace_InteractionFocusBorder, createColor(0, 0, 0, 100)); // TODO: Missing name!
	set(EColor::TutorialBgColor, createColor(232, 232, 232, 255));
	set(EColor::TutorialText, createColor(51, 51, 51, 255));
	set(EColor::TutorialBarBg, createColor(215, 215, 215, 255));
	set(EColor::TutorialScrollbarBg, createColor(215, 215, 215, 255));
	set(EColor::TutorialScrollbarActive, createColor(245, 245, 245, 255));
	set(EColor::TutorialScrollbarGrab, createColor(232, 232, 232, 255));
	set(EColor::TutorialScrollbarHovered, createColor(240, 240, 240, 255));
	set(EColor::TutorialTitleText, createColor(14, 98, 175, 255));
	set(EColor::TutorialHighlightText, createColor(51, 51, 51, 255));
	set(EColor::TutorialButtonText, createColor(255, 255, 255, 255));
	set(EColor::TutorialButtonBg, createColor(14, 98, 175, 255));
	set(EColor::TutorialButtonActive, createColor(19, 132, 230, 255));
	set(EColor::TutorialButtonHovered, createColor(19, 116, 201, 255));

	set(EColor::TutorialTaskBg, createColor(100, 100, 100, 25));

	set(EColor::SelectionRectFull, createColor(0, 0, 255, 100));
	set(EColor::SelectionRectTouch, createColor(0, 255, 0, 100));

	set(EColor::TrackingSequenceTint, ImVec4(1.2, 1.2, 1.2, 1));

	// Node Editor
	//	set(EColor::Nodes_FloatText, ImVec4(255.0f / 255.0f, 255.0f / 255.0f,
	// 255.0f / 255.0f, 1.00f)); 	set(EColor::NodeEditorBg, ImVec4(112.0f /
	// 255.0f, 112.0f / 255.0f, 116.0f / 255.0f, 1.00f));
	set(EColor::Nodes_ConnectionPossible, createColor(0, 255, 0, 255));
	set(EColor::Nodes_ConnectionNotPossible, createColor(255, 0, 0, 255));
	set(EColor::Nodes_CreateNode, createColor(32, 45, 32, 180));

	set(EColor::Builder_NodePadding, ImVec4(0, 0, 0, 0));

	// pin colors (background)
	set(EColor::PulsePin, createColor(164, 58, 190, 255));
	set(EColor::FloatPin, createColor(58, 144, 66, 255));
	set(EColor::MatrixPin, createColor(178, 71, 66, 255));
	set(EColor::QuatPin, createColor(178, 144, 66, 255));
	set(EColor::Vec3Pin, createColor(58, 84, 187, 255));
	set(EColor::Vec4Pin, createColor(106, 96, 67, 255));
	set(EColor::ScreenPin, createColor(51, 150, 215, 255));
	set(EColor::MatrixMulPin, createColor(68, 201, 156, 255));

	// pin inner colors {foreground)
	set(EColor::InnerPulsePin, createColor(255, 255, 255, 255));
	set(EColor::InnerFloatPin, createColor(255, 255, 255, 255));
	set(EColor::InnerMatrixPin, createColor(255, 255, 255, 255));
	set(EColor::InnerQuatPin, createColor(255, 255, 255, 255));
	set(EColor::InnerVec3Pin, createColor(255, 255, 255, 255));
	set(EColor::InnerVec4Pin, createColor(255, 255, 255, 255));
	set(EColor::InnerScreenPin, createColor(255, 255, 255, 255));
	set(EColor::InnerMatrixMulPin, createColor(255, 255, 255, 255));

	// General unspecified node
	set(EColor::NodeBg, createColor(255, 200, 50, 255));
	set(EColor::NodeHeader, createColor(0, 0, 0, 30));
	set(EColor::NodeFont, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

	// Sequence
	set(EColor::NodeBgSequence, ImVec4(0.541f, 0.541f, 0.541f, 1.0f));
	set(EColor::NodeHeaderSequence, ImVec4(0.431f, 0.431f, 0.431f, 1.0f));

	// Folta operator color set
	set(EColor::NodeBgOperator, ImVec4(83.0f / 255.0f, 101.0f / 255.0f, 146.0f / 255.0f, 1.00f));
	set(EColor::NodeHeaderOperator, ImVec4(73.0f / 255.0f, 91.0f / 255.0f, 136.0f / 255.0f, 1.00f));
	set(EColor::FloatBgOperator, ImVec4(77.0f / 255.0f, 85.0f / 255.0f, 106.0f / 255.0f, 1.00f));
	set(EColor::FloatBgOperatorActive, ImVec4(97.0f / 255.0f, 105.0f / 255.0f, 126.0f / 255.0f, 1.00f));
	set(EColor::FloatBgOperatorHovered, ImVec4(87.0f / 255.0f, 95.0f / 255.0f, 116.0f / 255.0f, 1.00f));

	// Folta transformation color set
	set(EColor::NodeBgTransformation, ImVec4(137.0f / 255.0f, 115.0f / 255.0f, 59.0f / 255.0f, 1.00f));
	set(EColor::NodeHeaderTranformation, ImVec4(0.498f, 0.412f, 0.192f, 1.00f));
	set(EColor::NodeBorder, ImVec4(0.0f, 0.0f, 0.0f, 0.10f));
	set(EColor::NodeLODButtonColorText, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	set(EColor::NodeLODButtonColor, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
	set(EColor::NodeLODButtonColorActive, ImVec4(0.0f, 0.0f, 0.0f, 0.05f));
	set(EColor::NodeLODButtonColorHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.20f));
	set(EColor::FloatBgTransformation, ImVec4(91.0f / 255.0f, 84.0f / 255.0f, 67.0f / 255.0f, 1.00f));
	set(EColor::FloatBgTransformationActive, ImVec4(111.0f / 255.0f, 104.0f / 255.0f, 87.0f / 255.0f, 1.00f));
	set(EColor::FloatBgTransformationHovered, createColor(101, 94, 77, 255));

	set(EColor::Links_selected_colorShift, ImVec4(0.2f, 0.2f, 0.2f, 0.0f));

	set(EColor::Synergies_FloatBg, ImVec4(0.0, 0.4f, 0.0f, 1.0f));
	set(EColor::Synergies_FloatBgHovered, ImVec4(0.0f, 0.4f, 0.2f, 1.0f));
	set(EColor::Synergies_FloatBgActive, ImVec4(0.0f, 0.4f, 0.4f, 1.0f));

	set(EColor::Nodes_Screen_resizeBtn_bgShape, ImVec4(0.70f, 0.70f, 0.70f, 0.5f));
	set(EColor::Nodes_Screen_resizeBtn_bgInner, ImVec4(0.70f, 0.70f, 0.70f, 0.5f));
	set(EColor::Nodes_Screen_resizeBtn_fgShape, ImVec4(1.0f, 1.0f, 1.0f, 0.82f));
	set(EColor::Nodes_Screen_resizeBtn_fgInner, ImVec4(1.0f, 1.0f, 1.0f, 0.82f));
	set(EColor::Nodes_Screen_noInput_background, ImVec4(0.0f, 0.0f, 0.0f, 0.65f));
	set(EColor::Nodes_Screen_noInput_text, ImVec4(1.0f, 1.0f, 1.0f, 0.65f));

	set(EColor::Nodes_Transformation_ValidIcon_bgShape, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	set(EColor::Nodes_Transformation_ValidIcon_bgInner, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	set(EColor::Nodes_Transformation_ValidIcon_fgShape, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	set(EColor::Nodes_Transformation_ValidIcon_fgInner, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	// set(EColor::Nodes_Transformation_ValidIcon_padding, ImVec4(5.0f, 5.0f, 5.0f, 5.0f));
	// is now multiplied by the fontSize in WorkspaceTransformation::topContent(), diwne.DrawIcon()
	// set(EColor::Nodes_Transformation_ValidIcon_padding, ImVec4(0.7216f, 0.7686f, 0.7255f, 0.7686f)); // for cross
	set(EColor::Nodes_Transformation_ValidIcon_padding, ImVec4(0.8745f, 0.7686f, 0.8863f, 0.7686f)); // for hyphen

	set(EColor::Nodes_Transformation_TrackingMarkColor, ImVec4(0.7f, 0.7f, 0.0f, 1.0f));
	set(EColor::Nodes_Transformation_TrackingColor, ImVec4(0.3f, 0.3f, 0.1f, 0.6f));

	set(EColor::StartWindow_DescriptionBackground, createColor(255, 255, 255, 255));
	set(EColor::StartWindow_WindowBackground, createColor(232, 232, 232, 255));
	set(EColor::StartWindow_ScrollbarBackground, createColor(232, 232, 232, 255));
	set(EColor::StartWindow_ScrollbarGrab, createColor(202, 202, 202, 255));
	set(EColor::StartWindow_ScrollbarGrabHovered, createColor(202, 202, 202, 255));
	set(EColor::StartWindow_ScrollbarGrabActive, createColor(202, 202, 202, 255));
	set(EColor::StartWindow_Separator, createColor(202, 202, 202, 255));
	set(EColor::StartWindow_DefaultButton, createColor(14, 98, 175, 255));
	set(EColor::StartWindow_NewSceneButton, createColor(8, 187, 230, 255));
	set(EColor::StartWindow_NewSceneButtonFont, createColor(255, 255, 255, 255));
	set(EColor::StartWindow_TitleFont, createColor(14, 98, 175, 255));
	set(EColor::StartWindow_DescriptionFont, createColor(65, 65, 66, 255));
	set(EColor::StartWindow_YourSceneWinBackground, createColor(255, 255, 255, 255));

	set(EColor::AboutWindow_BackgroundLeft, createColor(255, 255, 255, 255));
	set(EColor::AboutWindow_BackgroundRight, createColor(255, 255, 255, 255));
	set(EColor::AboutWindow_Text, createColor(0, 0, 0, 255));

	// Cycle - todo - this is modern too
	set(EColor::Cycle_Button, createColor(120, 120, 120, 255));
	set(EColor::Cycle_ButtonHovered, createColor(125, 125, 125, 255));
	set(EColor::Cycle_ButtonActive, createColor(130, 130, 130, 255));
	set(EColor::Cycle_ButtonForeground, createColor(0, 0, 0, 255));

	set(EColor::Cycle_RadioButton, createColor(33, 33, 33, 255));
	set(EColor::Cycle_RadioButtonActive, createColor(158, 220, 213, 255));
	set(EColor::Cycle_RadioButtonHovered, createColor(37, 37, 337, 255));
	set(EColor::Cycle_RadioButtonText, createColor(89, 89, 89, 255));
	set(EColor::Cycle_RadioButtonSelected, createColor(140, 190, 186, 255));
	set(EColor::Cycle_RadioButtonSelectedHovered, createColor(345, 195, 186, 255));
	set(EColor::Cycle_RadioButtonSelectedText, createColor(0, 0, 0, 255));
	set(EColor::Cycle_RadioButtonBackground, createColor(33, 33, 33, 255));


	m_fontsAssoc.insert(std::pair(EFont::MenuLarge, "Roboto14"));
	m_fontsAssoc.insert(std::pair(EFont::Button, "RobotoBold12"));
	m_fontsAssoc.insert(std::pair(EFont::Tab, "RobotoBold12"));
	m_fontsAssoc.insert(std::pair(EFont::Node, "Roboto12"));
	m_fontsAssoc.insert(std::pair(EFont::MenuSmall, "Roboto12"));
	m_fontsAssoc.insert(std::pair(EFont::Header, "RobotoBold20"));

	// Tutorial fonts.
	m_fontsAssoc.insert(std::pair(EFont::TutorialText, "Roboto16"));
	m_fontsAssoc.insert(std::pair(EFont::TutorialTitle, "UbuntuBold24"));
	m_fontsAssoc.insert(std::pair(EFont::TutorialAssignment, "RobotoBold16"));
	m_fontsAssoc.insert(std::pair(EFont::TutorialHint, "RobotoItalic16"));
	m_fontsAssoc.insert(std::pair(EFont::WelcomeTitle, "UbuntuBold33.5"));
	m_fontsAssoc.insert(std::pair(EFont::WelcomeDescription, "Roboto17.5"));
	m_fontsAssoc.insert(std::pair(EFont::WelcomeItemTitle, "UbuntuBold18"));
	m_fontsAssoc.insert(std::pair(EFont::WelcomeItemDescription, "Roboto12"));

	m_sizes[ESize::Nodes_FloatMargin] = 1.0f;
	m_sizes[ESize::Nodes_FloatWidth] = 25.0f;
	m_sizes[ESize::Nodes_Rounding] = 5.0f;
	m_sizes[ESize::Nodes_BorderWidth] = 0.0f;
	m_sizes[ESize::Nodes_LabelIndent] = 3.0f;
	m_sizes[ESize::Nodes_HeaderLabelIndent] = 2.0f;
	m_sizes[ESize::Nodes_trackballButtonHeight] = 20.0f;
	m_sizes[ESize::Nodes_TrackBallSensitivity] = 5.0f;

	m_sizes[ESize::Nodes_FloatInnerPadding] = 1.0f;

	m_sizes[ESize::Nodes_dragSpeedDefaulrRatio] = 0.015f;
	m_sizes[ESize::Nodes_CtrlMultiplicator] = 0.1f;
	m_sizes[ESize::Nodes_SHIFTMultiplicator] = 10.0f;
	m_sizes[ESize::Nodes_ALTMultiplicator] = 0.01f;

	m_sizes[ESize::Nodes_InputsAlignment] = 0.0f;
	m_sizes[ESize::Nodes_MiddleAlignment] = 0.0f;
	m_sizes[ESize::Nodes_OutputsAlignment] = 0.0f;

	m_sizes[ESize::Nodes_leftSideSpacing] = 3.0f;
	m_sizes[ESize::Nodes_rightSideSpacing] = 3.0f;

	m_sizes[ESize::Workspace_SelectedBorderThickness] = 2.5f;
	m_sizes[ESize::Workspace_FocusBorderThickness] = 1.5f;
	m_sizes[ESize::Workspace_InteractionFocusBorderThickness] = 1.5f;
	m_sizes[ESize::Workspace_CopyPasteOffset] = 25.f;
	m_sizes[ESize::Workspace_TrackingTimeBetweenTracks] = 0.0005f;

	m_sizes[ESize::TutorialTaskSquareXPadding] = 10.0f;
	m_sizes[ESize::TutorialWindow_FrameRounding] = 5.0f;
	m_sizes[ESize::TutorialWindow_ScrollbarRounding] = 5.0f;
	m_sizes[ESize::TutorialWindow_ScrollbarSize] = 15.0f;
	m_sizes[ESize::TutorialWindow_BackButtonWidth] = 40.0f;
	m_sizes[ESize::TutorialWindow_MainMenuButtonWidth] = 120.0f;

	m_sizes[ESize::Default_VisiblePrecision] = 1.0f;
	m_sizes[ESize::Default_VisibleQuaternionPrecision] = 4.0f;

	m_sizes[ESize::Default_InactiveMark] = 0.0f;

	m_sizes[ESize::Links_ControlpointsPositionFraction] = 0.2f;
	m_sizes[ESize::Links_ControlpointsPositionMin] = 50.0f;
	m_sizes[ESize::Links_Thickness] = 5.0;
	m_sizes[ESize::Links_ThicknessSelectedBorder] = 2.0;

	m_sizes[ESize::Pins_IconPadding] = 2.0;
	m_sizes[ESize::Links_selected_alpha] = 0.8;

	m_sizes[ESize::Float_inactive_alphaMultiplicator] = 0.5;

	m_sizes[ESize::Nodes_Operators_Rounding] = 5.0;
	m_sizes[ESize::Nodes_Sequence_Rounding] = 5.0;
	m_sizes[ESize::Nodes_LOD_Button_Rounding] = 5.0;
	m_sizes[ESize::Nodes_Border_Rounding] = 5.0;
	m_sizes[ESize::Nodes_Border_Thickness] = 1.5;

	m_sizes[ESize::Nodes_Transformation_TrackingMarkSize] = 5.f;

	m_sizes[ESize::Tracking_SmoothScrollSpeed] = 0.03;
	m_sizes[ESize::Tracking_JaggedScrollSpeed] = 0.2;

	m_sizes[ESize::Window_Rounding] = 0.0f;
	m_sizes[ESize::Tooltip_Rounding] = 10.0f;

	m_sizes[ESize::StartWindow_WinWidth] = 850.0f;
	m_sizes[ESize::StartWindow_WinHeight] = 500.0f;
	m_sizes[ESize::StartWindow_WinRounding] = 6.0f;
	m_sizes[ESize::StartWindow_TitleVerticalOffset] = 130.0f;
	m_sizes[ESize::StartWindow_LeftBarWidth] = 330.0f;
	m_sizes[ESize::StartWindow_LoadButtonWidth] = 120.0f;
	m_sizes[ESize::StartWindow_StartNewButtonWidth] = 120.0f;
	m_sizes[ESize::StartWindow_ButtonHeight] = 30.0f;
	m_sizes[ESize::StartWindow_ThumbImageSize] = 80.0f;
	m_sizes[ESize::StartWindow_StartButtonWidth] = 120.0f;
	m_sizes[ESize::StartWindow_FrameRounding] = 4.0f;
	m_sizes[ESize::StartWindow_ScrollbarSize] = 14.0f;
	m_sizes[ESize::StartWindow_YourSceneWinRounding] = 6.0f;
	m_sizes[ESize::StartWindow_DotSize] = 10.0f;
	m_sizes[ESize::StartWindow_DotSpacing] = 7.0f;

	m_sizes[ESize::Cycle_ButtonRounding] = 3.0f;
	m_sizes[ESize::Cycle_RadioButtonRounding] = 5.0f;

	m_sizesVec2[ESizeVec2::Window_FramePadding] = ImVec2(4.0f, 4.0f);

	m_sizesVec2[ESizeVec2::Nodes_ItemsSpacing] = ImVec2(2.0f, 3.0f);
	m_sizesVec2[ESizeVec2::Nodes_FloatPadding] = ImVec2(0.0f, 1.0f);
	m_sizesVec2[ESizeVec2::Nodes_PinSpacing] = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_PivotAlignment] = ImVec2(0.0f, 0.5f);
	m_sizesVec2[ESizeVec2::Nodes_PivotSize] = ImVec2(0.0f, 0.0f);

	m_sizesVec2[ESizeVec2::Nodes_InputsSize] = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_MiddleSize] = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_OutputSize] = ImVec2(0.0f, 0.0f);

	m_sizesVec2[ESizeVec2::Nodes_LODButtonSize] = ImVec2(25.0f, 25.0f);

	m_sizesVec2[ESizeVec2::Nodes_IconSize] = ImVec2(12.0f, 12.0f);
	m_sizesVec2[ESizeVec2::Nodes_FloatCycleButtonSize] = ImVec2(32.0f, 32.0f);
	m_sizesVec2[ESizeVec2::Nodes_ScreenTextureSize] = ImVec2(130.0f, 130.0f);

	m_sizesVec2[ESizeVec2::Builder_ItemSpacing] = ImVec2(0.0f, 0.0f);

	m_sizesVec2[ESizeVec2::Nodes_Screen_resizeButtonSize] = ImVec2(20.f, 20.f);

	m_sizesVec2[ESizeVec2::Nodes_Sequence_DummySpaceSize] = ImVec2(100.f, 1.f);

	m_sizesVec2[ESizeVec2::Nodes_noPinsSpacing] = ImVec2(0.f, 20.f);

	m_sizesVec2[ESizeVec2::NewNode_positionShift] = ImVec2(10.f, 0.f);

	m_sizesVec2[ESizeVec2::TutorialWindow_Padding] = ImVec2(30.f, 35.f);
	m_sizesVec2[ESizeVec2::Tooltip_Padding] = ImVec2(10.f, 10.f);
	m_sizesVec2[ESizeVec2::Window_Padding] = ImVec2(0.f, 0.f);

	m_sizesVec2[ESizeVec2::StartWindow_WinSize] = ImVec2(1020.f, 600.f);
	m_sizesVec2[ESizeVec2::StartWindow_LogoOffset] = ImVec2(5.f, -20.f);
	m_sizesVec2[ESizeVec2::StartWindow_WinPadding] = ImVec2(0.f, 0.f);
	m_sizesVec2[ESizeVec2::StartWindow_LeftWinPadding] = ImVec2(30.f, 30.f);
	m_sizesVec2[ESizeVec2::StartWindow_RightWinOuterPadding] = ImVec2(10.f, 10.f);
	m_sizesVec2[ESizeVec2::StartWindow_RightWinInnerPadding] = ImVec2(10.f, 10.f);

	m_sizesVec2[ESizeVec2::Cycle_ButtonSize] = ImVec2(40.0f, 40.0f);
}

void Theme::initNames()
{
	group("Global")
	    .add(EColor::PrimaryColor, "Primary Color (tabs, tiles, ...)")
	    .add(EColor::ActiveColor, "Active Color")
	    .add(EColor::SelectionColor, "Selection Color")
	    .add(EColor::Text, "Text")
	    .add(EColor::TextDisabled, "Disabled Text")
	    .add(EColor::WindowBackground, "Window Background")
	    .add(EColor::PopupBackground, "Popup Background")
	    .add(EColor::MenuBarBackground, "Menu Bar Background")
	    .add(EColor::SceneViewBackground, "Tab SceneView Background")
	    // Frames and borders
	    .add(EColor::Border, "Border")
	    .add(EColor::FrameBg, "Frame Background")
	    .add(EColor::FrameBgHovered, "Frame Background Hovered")
	    .add(EColor::FrameBgActive, "Frame Background Active")
	    // Tabs
	    .add(EColor::Tab, "Tab Color")
	    .add(EColor::TabHovered, "Tab Hovered")
	    .add(EColor::TabActive, "Tab Active")
	    // Dock tab options
	    .add(EColor::DockTab, "Dock Tab")
	    .add(EColor::DockTabActive, "Dock Active")
	    .add(EColor::DockTabUnfocused, "Dock Unfocused")
	    .add(EColor::DockTabUnfocusedActive, "Dock Unfocused Active")
	    .add(EColor::DockTabHovered, "Dock Tab Hovered")
	    .add(EColor::SelectionRectFull, "Selection rectangle full")
	    .add(EColor::SelectionRectTouch, "Selection rectangle touch")
	    .add(EColor::Workspace_SelectedBorder, "Selected node border")
	    .add(ESize::Workspace_SelectedBorderThickness, "Selected node border thickness")
	    .add(ESizeVec2::Window_FramePadding, "Windows Frame Padding");

	group("Node Editor")
	    .add(EColor::NodeBg, "General Node Background")
	    .add(EColor::NodeHeader, "General Node Header")
	    .add(EColor::NodeFont, "General Node Font (text)")
	    .add(EColor::NodeBorder, "Node Border")
	    .add(EColor::NodeLODButtonColorText, "LOD Button Text")
	    .add(EColor::NodeLODButtonColor, "LOD Button Color")
	    .add(EColor::NodeLODButtonColorActive, "LOD Button Active Color")
	    .add(EColor::NodeLODButtonColorHovered, "LOD Button Hover Color")
	    .add(ESize::Float_inactive_alphaMultiplicator, "Float_inactive_alphaMultiplicator")
	    .add(ESize::Nodes_Operators_Rounding, "Nodes Operators Rounding")
	    .add(ESize::Nodes_Sequence_Rounding, "Nodes Sequence Rounding")
	    .add(ESize::Nodes_LOD_Button_Rounding, "Nodes LOD Button Rounding")
	    .add(ESize::Nodes_Border_Rounding, "Nodes Border Rounding")
	    .add(ESize::Nodes_Border_Thickness, "Nodes Border Thickness")
	    .add(ESize::Nodes_Rounding, "Nodes Rounding")
	    .add(ESize::Nodes_FloatWidth, "Nodes Float Width")
	    .add(ESize::Nodes_FloatMargin, "Nodes Float Margin")
	    .add(ESize::Nodes_BorderWidth, "Nodes BorderWidth")
	    .add(ESize::Nodes_LabelIndent, "Pin Label Indent")
	    .add(ESize::Nodes_HeaderLabelIndent, "Header Label Indent")
	    .add(ESize::Nodes_trackballButtonHeight, "Trackball Button Height")
	    .add(ESize::Nodes_TrackBallSensitivity, "TrackBall Sensitivity")
	    .add(ESize::Nodes_FloatInnerPadding, "Float Inner Padding")
	    .add(ESize::Nodes_dragSpeedDefaulrRatio, "Drag Speed fo Float")
	    .add(ESize::Nodes_CtrlMultiplicator, "CTRL Multiplicator")
	    .add(ESize::Nodes_SHIFTMultiplicator, "SHIFT Multiplicator")
	    .add(ESize::Nodes_ALTMultiplicator, "ALT Multiplicator")
	    .add(ESize::Nodes_InputsAlignment, "Nodes Inputs Alignment")
	    .add(ESize::Nodes_MiddleAlignment, "Nodes Middle Alignment")
	    .add(ESize::Nodes_OutputsAlignment, "Nodes Outputs Alignment")
	    .add(ESize::Nodes_leftSideSpacing, "Nodes Left Side spacing")
	    .add(ESize::Nodes_rightSideSpacing, "Nodes Right Side spacing")
	    .add(ESize::Nodes_Transformation_TrackingMarkSize, "Nodes Transformation TrackingMarkSize")
	    .add(ESize::Default_VisiblePrecision, "Nodes Default Visible Precision")
	    .add(ESize::Default_VisibleQuaternionPrecision, "Nodes Default Visible Precision For Quaternions")
	    .add(ESize::Default_InactiveMark, "Nodes Default Inactive Part Marker")
	    .add(ESize::Links_ControlpointsPositionFraction, "Link x distance between controlpoint and start/end of link - "
	                                                     "fraction of start to end x distance")
	    .add(ESize::Links_ControlpointsPositionMin,
	         "Link minimal x distance between controlpoint and start/end of link")
	    .add(ESize::Links_Thickness, "Link Thickness")
	    .add(ESize::Links_ThicknessSelectedBorder, "Link Additional thickness when selected")
	    .add(ESize::Links_selected_alpha, "Link Selected Alpha")
	    .add(ESize::Pins_IconPadding, "Pin Icon Padding")
	    .add(ESizeVec2::Nodes_Screen_resizeButtonSize, "Screen Resize Button Size")
	    .add(ESizeVec2::Nodes_Sequence_DummySpaceSize, "Sequence Dummy Space Size")
	    .add(ESizeVec2::NewNode_positionShift, "New Node Position Shift")
	    .add(ESizeVec2::Nodes_IconSize, "Nodes Icon Size")
	    .add(ESizeVec2::Nodes_PivotAlignment, "Nodes Pivot Alignment")
	    .add(ESizeVec2::Nodes_PinSpacing, "Nodes Pin Spacing")
	    .add(ESizeVec2::Nodes_ItemsSpacing, "Nodes Items Spacing")
	    .add(ESizeVec2::Nodes_FloatPadding, "Nodes Float Padding")
	    .add(ESizeVec2::Nodes_PivotSize, "Nodes Pivot Size")
	    .add(ESizeVec2::Nodes_InputsSize, "Nodes Inputs Size")
	    .add(ESizeVec2::Nodes_MiddleSize, "Nodes Middle Size")
	    .add(ESizeVec2::Nodes_OutputSize, "Nodes Output Size")
	    .add(ESizeVec2::Nodes_LODButtonSize, "Nodes LOD Button Size")
	    .add(ESizeVec2::Nodes_FloatCycleButtonSize, "Nodes Float Cycle Button Size")
	    .add(ESizeVec2::Nodes_ScreenTextureSize, "Nodes Screen Texture Size")
	    .add(ESizeVec2::Builder_ItemSpacing, "Builder Item Spacing");

	group("Node Editor Pins")
	    .add(EColor::PulsePin, "Pulse Pin")
	    .add(EColor::FloatPin, "Float Pin")
	    .add(EColor::MatrixPin, "Matrix Pin")
	    .add(EColor::QuatPin, "Quaternion Pin")
	    .add(EColor::Vec3Pin, "Vector 3 Pin")
	    .add(EColor::Vec4Pin, "Vector 4 Pin")
	    .add(EColor::ScreenPin, "Screen Pin")
	    .add(EColor::MatrixMulPin, "MatrixMul Pin")
	    .add(EColor::InnerPulsePin, "Inner Color Pulse")
	    .add(EColor::InnerFloatPin, "Inner Color Float")
	    .add(EColor::InnerMatrixPin, "Inner Color Matrix")
	    .add(EColor::InnerQuatPin, "Inner Color Quat")
	    .add(EColor::InnerVec3Pin, "Inner Color Vec3")
	    .add(EColor::InnerVec4Pin, "Inner Color Vec4")
	    .add(EColor::InnerScreenPin, "Inner Color Screen")
	    .add(EColor::InnerMatrixMulPin, "Inner Color MatrixMul");

	group("Node Editor Operators")
	    .add(EColor::NodeBgOperator, "Operator Background")
	    .add(EColor::NodeHeaderOperator, "Operator Header")
	    .add(EColor::FloatBgOperator, "Operator Float Background")
	    .add(EColor::FloatBgOperatorActive, "Operator Active Float Background")
	    .add(EColor::FloatBgOperatorHovered, "Operator Hovered Float Background");

	group("Node Editor Transformations")
	    .add(EColor::NodeBgTransformation, "Transform Background")
	    .add(EColor::NodeHeaderTranformation, "Transform Header")
	    .add(EColor::FloatBgTransformation, "Transform Float Background")
	    .add(EColor::FloatBgTransformationActive, "Transform Float Active")
	    .add(EColor::FloatBgTransformationHovered, "Transform Float Hovered")
	    .add(EColor::Nodes_Transformation_ValidIcon_bgShape, "Nodes Transformation ValidIcon bgShape")
	    .add(EColor::Nodes_Transformation_ValidIcon_bgInner, "Nodes Transformation ValidIcon bgInner")
	    .add(EColor::Nodes_Transformation_ValidIcon_fgShape, "Nodes Transformation ValidIcon fgShape")
	    .add(EColor::Nodes_Transformation_ValidIcon_fgInner, "Nodes Transformation ValidIcon fgInner")
	    .add(EColor::Nodes_Transformation_ValidIcon_padding, "Nodes Transformation ValidIcon padding")
	    .add(EColor::Nodes_Transformation_TrackingColor, "Nodes Transformation TrackingColor")
	    .add(EColor::Nodes_Transformation_TrackingMarkColor, "Nodes Transformation TrackingMarkColor");

	group("Node Editor Popups")
	    .add(EColor::Nodes_Screen_resizeBtn_bgShape, "Nodes Screen resizeBtn bgShape")
	    .add(EColor::Nodes_Screen_resizeBtn_bgInner, "Nodes Screen resizeBtn bgInner")
	    .add(EColor::Nodes_Screen_resizeBtn_fgShape, "Nodes Screen resizeBtn fgShape")
	    .add(EColor::Nodes_Screen_resizeBtn_fgInner, "Nodes Screen resizeBtn fgInner")
	    .add(EColor::Nodes_Screen_noInput_background, "Nodes Screen noInput background")
	    .add(EColor::Nodes_Screen_noInput_text, "Nodes Screen noInput text");

	group("Node Editor Floats")
	    .add(EColor::Synergies_FloatBg, "Synergies FloatBg")
	    .add(EColor::Synergies_FloatBgHovered, "Synergies FloatBgHovered")
	    .add(EColor::Synergies_FloatBgActive, "Synergies FloatBgActive");

	group("Node Editor ScreenNode")
	    .add(EColor::Nodes_Screen_resizeBtn_bgShape, "Nodes Screen resizeBtn bgShape")
	    .add(EColor::Nodes_Screen_resizeBtn_bgInner, "Nodes Screen resizeBtn bgInner")
	    .add(EColor::Nodes_Screen_resizeBtn_fgShape, "Nodes Screen resizeBtn fgShape")
	    .add(EColor::Nodes_Screen_resizeBtn_fgInner, "Nodes Screen resizeBtn fgInner")
	    .add(EColor::Nodes_Screen_noInput_background, "Nodes_Screen_noInput_background")
	    .add(EColor::Nodes_Screen_noInput_text, "Nodes_Screen_noInput_text");

	group("Node Editor Cycle")
	    // large button icons
	    .add(EColor::Cycle_Button, "Button")
	    .add(EColor::Cycle_ButtonHovered, "ButtonHovered")
	    .add(EColor::Cycle_ButtonActive, "ButtonActive")
	    .add(EColor::Cycle_ButtonForeground, "ButtonForeground")
	    .add(ESize::Cycle_ButtonRounding, "ButtonRounding")
	    .add(ESizeVec2::Cycle_ButtonSize, "ButtonSize")
	    // radio buttons
	    .add(EColor::Cycle_RadioButton, "RadioButton")
	    .add(EColor::Cycle_RadioButtonActive, "RadioButtonActive")
	    .add(EColor::Cycle_RadioButtonHovered, "RadioButtonHovered")
	    .add(EColor::Cycle_RadioButtonText, "RadioButtonText")
	    .add(EColor::Cycle_RadioButtonSelected, "RadioButtonSelected")
	    .add(EColor::Cycle_RadioButtonSelectedHovered, "RadioButtonSelectedHovered")
	    .add(EColor::Cycle_RadioButtonSelectedText, "RadioButtonSelectedText")
	    .add(EColor::Cycle_RadioButtonBackground, "RadioButtonBackground")
	    .add(ESize::Cycle_RadioButtonRounding, "RadioButtonRounding");

	/*

	 */

	group("Node Editor Link").add(EColor::Links_selected_colorShift, "selected_colorShift");

	checkThemeVariables();

	// All category keys must be I3T_PROPERTY_NAME_OFFSET characters long.
	/// \todo This will be removed.
	g_CategoryNames["glob_"] = "Global";
	g_CategoryNames["tuts_"] = "Tutorials";
	g_CategoryNames["star_"] = "Start Window";
	g_CategoryNames["abut_"] = "About Window";
	g_CategoryNames["npin_"] = "Node Editor Pins";
	g_CategoryNames["ngen_"] = "Node Editor General";
	g_CategoryNames["nops_"] = "Node Editor Operators";
	g_CategoryNames["ntrs_"] = "Node Editor Transformations";
	g_CategoryNames["npop_"] = "Node Editor Popups";
	g_CategoryNames["nflo_"] = "Node Editor Floats";
	g_CategoryNames["nscr_"] = "Node Editor ScreenNode";
	g_CategoryNames["nlnk_"] = "Node Editor Link";
	g_CategoryNames["ncyc_"] = "Node Editor Cycle";

	// Global
	g_ColorNames[EColor::PrimaryColor] = "glob_Primary Color (tabs, tiles, ...)";
	g_ColorNames[EColor::ActiveColor] = "glob_Active Color";
	g_ColorNames[EColor::SelectionColor] = "glob_Selection Color";

	g_ColorNames[EColor::Text] = "glob_Text";
	g_ColorNames[EColor::TextDisabled] = "glob_Text Disabled";
	g_ColorNames[EColor::WindowBackground] = "glob_Window Background";
	g_ColorNames[EColor::PopupBackground] = "glob_Popup Background";
	g_ColorNames[EColor::Border] = "glob_Border";
	g_ColorNames[EColor::FrameBg] = "glob_Frame Background";
	g_ColorNames[EColor::FrameBgHovered] = "glob_Frame Background (Hovered)";
	g_ColorNames[EColor::FrameBgActive] = "glob_Frame Background (Active)";
	g_ColorNames[EColor::MenuBarBackground] = "glob_Menu Bar Background";
	g_ColorNames[EColor::Button] = "glob_Button";
	g_ColorNames[EColor::ButtonHovered] = "glob_Button (Hovered)";
	g_ColorNames[EColor::ButtonActive] = "glob_Button (Active)";
	g_ColorNames[EColor::Tab] = "glob_Tab";
	g_ColorNames[EColor::TabHovered] = "glob_Tab (Hovered)";
	g_ColorNames[EColor::TabActive] = "glob_Tab (Active)";

	g_ColorNames[EColor::DockTab] = "glob_Dock Tab Color";
	g_ColorNames[EColor::DockTabActive] = "glob_Dock Selected Tab Color";
	g_ColorNames[EColor::DockTabUnfocused] = "glob_Dock Unfocused Tab Color";
	g_ColorNames[EColor::DockTabUnfocusedActive] = "glob_Dock Unfocused Selected Tab Color";
	g_ColorNames[EColor::DockTabHovered] = "glob_Dock Hovered Tab Color";

	g_ColorNames[EColor::SceneViewBackground] = "glob_Tab SceneView Background";
	g_ColorNames[EColor::SelectionRectFull] = "glob_Selection rectangle full";
	g_ColorNames[EColor::SelectionRectTouch] = "glob_Selection rectangle touch";
	g_ColorNames[EColor::Workspace_SelectedBorder] = "glob_Selected node border";

	g_SizeNames[ESize::Workspace_SelectedBorderThickness] = "glob_Selected node border thickness";
	g_SizeVecNames[ESizeVec2::Window_FramePadding] = "glob_Windows Frame Padding";

	// Tutorials
	/// \todo This will be removed.
	g_ColorNames[EColor::TutorialBgColor] = "tuts_Background";
	g_ColorNames[EColor::TutorialText] = "tuts_Text";
	g_ColorNames[EColor::TutorialBarBg] = "tuts_Bar Background";
	g_ColorNames[EColor::TutorialScrollbarBg] = "tuts_Scrollbar Background";
	g_ColorNames[EColor::TutorialScrollbarActive] = "tuts_Scrollbar Active";
	g_ColorNames[EColor::TutorialScrollbarGrab] = "tuts_Scrollbar Grab";
	g_ColorNames[EColor::TutorialScrollbarHovered] = "tuts_Scrollbar Hovered";
	g_ColorNames[EColor::TutorialTitleText] = "tuts_Title Text";
	g_ColorNames[EColor::TutorialHighlightText] = "tuts_Highlight Text";
	g_ColorNames[EColor::TutorialButtonText] = "tuts_Button Text";
	g_ColorNames[EColor::TutorialButtonBg] = "tuts_Button Background";
	g_ColorNames[EColor::TutorialButtonActive] = "tuts_Button Active";
	g_ColorNames[EColor::TutorialButtonHovered] = "tuts_Button Hovered";
	g_ColorNames[EColor::TutorialTaskBg] = "tuts_Task Background";


	// Start Window
	/// \todo This will be removed.
	g_ColorNames[EColor::StartWindow_DescriptionBackground] = "star_Description Background";
	g_ColorNames[EColor::StartWindow_WindowBackground] = "star_Window Background";
	g_ColorNames[EColor::StartWindow_ScrollbarBackground] = "star_Scrollbar Background";
	g_ColorNames[EColor::StartWindow_ScrollbarGrab] = "star_Scrollbar Grab";
	g_ColorNames[EColor::StartWindow_ScrollbarGrabHovered] = "star_Scrollbar Grab Hovered";
	g_ColorNames[EColor::StartWindow_ScrollbarGrabActive] = "star_Scrollbar Grab Active";
	g_ColorNames[EColor::StartWindow_Separator] = "star_Separator";
	g_ColorNames[EColor::StartWindow_DefaultButton] = "star_Default Button";
	g_ColorNames[EColor::StartWindow_NewSceneButton] = "star_New Scene Button";
	g_ColorNames[EColor::StartWindow_NewSceneButtonFont] = "star_New Scene Button Font";
	g_ColorNames[EColor::StartWindow_TitleFont] = "star_Title Font Color";
	g_ColorNames[EColor::StartWindow_DescriptionFont] = "star_Description Font Color";
	g_ColorNames[EColor::StartWindow_YourSceneWinBackground] = "star_Your Scene Section Background";

	/// \todo This will be removed.
	g_SizeNames[ESize::StartWindow_WinWidth] = "star_Window Width";
	g_SizeNames[ESize::StartWindow_WinHeight] = "star_Window Height";
	g_SizeNames[ESize::StartWindow_WinRounding] = "star_Window Rounding";
	g_SizeNames[ESize::StartWindow_TitleVerticalOffset] = "star_Title Vertical Offset";
	g_SizeNames[ESize::StartWindow_LeftBarWidth] = "star_Left Bar Width";
	g_SizeNames[ESize::StartWindow_LoadButtonWidth] = "star_Load Button Width";
	g_SizeNames[ESize::StartWindow_StartNewButtonWidth] = "star_Start New Button Width";
	g_SizeNames[ESize::StartWindow_ButtonHeight] = "star_Button Height";
	g_SizeNames[ESize::StartWindow_ThumbImageSize] = "star_I3T Image Size";
	g_SizeNames[ESize::StartWindow_StartButtonWidth] = "star_Start Button Width";
	g_SizeNames[ESize::StartWindow_FrameRounding] = "star_Frame Rounding";
	g_SizeNames[ESize::StartWindow_ScrollbarSize] = "star_Scrollbar Size";
	g_SizeNames[ESize::StartWindow_YourSceneWinRounding] = "star_Your Scene Section Rounding";
	g_SizeNames[ESize::StartWindow_DotSize] = "star_Dot Size";
	g_SizeNames[ESize::StartWindow_DotSpacing] = "star_Dot Spacing";

	/// \todo This will be removed.
	g_SizeVecNames[ESizeVec2::StartWindow_WinSize] = "star_Window Size";
	g_SizeVecNames[ESizeVec2::StartWindow_LogoOffset] = "star_Logo Offset";
	g_SizeVecNames[ESizeVec2::StartWindow_WinPadding] = "star_Window Padding";
	g_SizeVecNames[ESizeVec2::StartWindow_LeftWinPadding] = "star_Left Window Padding";
	g_SizeVecNames[ESizeVec2::StartWindow_RightWinOuterPadding] = "star_Right Window OuterPadding";
	g_SizeVecNames[ESizeVec2::StartWindow_RightWinInnerPadding] = "star_Right Window InnerPadding";

	// About Window
	/// \todo This will be removed.
	g_ColorNames[EColor::AboutWindow_BackgroundLeft] = "abut_AboutWindow BackgroundLeft";
	g_ColorNames[EColor::AboutWindow_BackgroundRight] = "abut_AboutWindow BackgroundRight";
	g_ColorNames[EColor::AboutWindow_Text] = "abut_AboutWindow Text";

	// Node editor colors
	// ------------------
	/// \todo This will be removed.

	// 1. Pins
	g_ColorNames[EColor::PulsePin] = "npin_Pulse Pin";
	g_ColorNames[EColor::FloatPin] = "npin_Float Pin";
	g_ColorNames[EColor::MatrixPin] = "npin_Matrix Pin";
	g_ColorNames[EColor::QuatPin] = "npin_Quaternion Pin";
	g_ColorNames[EColor::Vec3Pin] = "npin_Vector 3 Pin";
	g_ColorNames[EColor::Vec4Pin] = "npin_Vector 4 Pin";
	g_ColorNames[EColor::MatrixMulPin] = "npin_MatrixMul Pin";
	g_ColorNames[EColor::ScreenPin] = "npin_Screen Pin";

	g_ColorNames[EColor::InnerPulsePin] = "npin_Inner Color Pulse";
	g_ColorNames[EColor::InnerFloatPin] = "npin_Inner Color Float";
	g_ColorNames[EColor::InnerVec3Pin] = "npin_Inner Color Vec3";
	g_ColorNames[EColor::InnerVec4Pin] = "npin_Inner Color Vec4";
	g_ColorNames[EColor::InnerMatrixPin] = "npin_Inner Color Matrix";
	g_ColorNames[EColor::InnerQuatPin] = "npin_Inner Color Quat";
	g_ColorNames[EColor::InnerMatrixMulPin] = "npin_Inner Color MatrixMul";
	g_ColorNames[EColor::InnerScreenPin] = "npin_Inner Color Screen";


	// 2. General unspecified node
	g_ColorNames[EColor::NodeBg] = "ngen_General Node Background";
	g_ColorNames[EColor::NodeHeader] = "ngen_General Node Header";
	g_ColorNames[EColor::NodeFont] = "ngen_General Node Font (text)";
	g_ColorNames[EColor::NodeBorder] = "ngen_Node Border";
	g_ColorNames[EColor::NodeLODButtonColorText] = "ngen_LOD Button Text";
	g_ColorNames[EColor::NodeLODButtonColor] = "ngen_LOD Button Color";
	g_ColorNames[EColor::NodeLODButtonColorActive] = "ngen_LOD Button Active Color";
	g_ColorNames[EColor::NodeLODButtonColorHovered] = "ngen_LOD Button Hover Color";

	// 3. Operator
	g_ColorNames[EColor::NodeBgOperator] = "nops_Operator Background";
	g_ColorNames[EColor::NodeHeaderOperator] = "nops_Operator Header";
	g_ColorNames[EColor::FloatBgOperator] = "nops_Operator Float Background";
	g_ColorNames[EColor::FloatBgOperatorActive] = "nops_Operator Active Float Background";
	g_ColorNames[EColor::FloatBgOperatorHovered] = "nops_Operator Hovered Float Background";

	// 4. Transforms
	g_ColorNames[EColor::NodeBgTransformation] = "ntrs_Transform Background";
	g_ColorNames[EColor::NodeHeaderTranformation] = "ntrs_Transform Header";
	g_ColorNames[EColor::FloatBgTransformation] = "ntrs_Transform Float Background";
	g_ColorNames[EColor::FloatBgTransformationActive] = "ntrs_Transform Float Active";
	g_ColorNames[EColor::FloatBgTransformationHovered] = "ntrs_Transform Float Hovered";
	g_ColorNames[EColor::Nodes_Transformation_ValidIcon_bgShape] = "ntrs_Nodes Transformation ValidIcon bgShape";
	g_ColorNames[EColor::Nodes_Transformation_ValidIcon_bgInner] = "ntrs_Nodes Transformation ValidIcon bgInner";
	g_ColorNames[EColor::Nodes_Transformation_ValidIcon_fgShape] = "ntrs_Nodes Transformation ValidIcon fgShape";
	g_ColorNames[EColor::Nodes_Transformation_ValidIcon_fgInner] = "ntrs_Nodes Transformation ValidIcon fgInner";
	// Padding stored as color - is multiplied by the fontSize in WorkspaceTransformation::topContent() diwne.DrawIcon()
	g_ColorNames[EColor::Nodes_Transformation_ValidIcon_padding] = "ntrs_Nodes Transformation ValidIcon padding";
	g_ColorNames[EColor::Nodes_Transformation_TrackingColor] = "ntrs_Nodes Transformation TrackingColor";

	// 5. Popups
	g_ColorNames[EColor::Nodes_ConnectionPossible] = "npop_Connection is possible (text)";
	g_ColorNames[EColor::Nodes_ConnectionNotPossible] = "npop_Connection is not possible (text)";
	g_ColorNames[EColor::Nodes_CreateNode] = "npop_Create node popup";

	// Floats
	g_ColorNames[EColor::Synergies_FloatBg] = "nflo_Synergies FloatBg";
	g_ColorNames[EColor::Synergies_FloatBgHovered] = "nflo_Synergies FloatBgHovered";
	g_ColorNames[EColor::Synergies_FloatBgActive] = "nflo_Synergies FloatBgActive";

	// Screen
	g_ColorNames[EColor::Nodes_Screen_resizeBtn_bgShape] = "nscr_Nodes Screen resizeBtn bgShape";
	g_ColorNames[EColor::Nodes_Screen_resizeBtn_bgInner] = "nscr_Nodes Screen resizeBtn bgInner";
	g_ColorNames[EColor::Nodes_Screen_resizeBtn_fgShape] = "nscr_Nodes Screen resizeBtn fgShape";
	g_ColorNames[EColor::Nodes_Screen_resizeBtn_fgInner] = "nscr_Nodes Screen resizeBtn fgInner";

	g_ColorNames[EColor::Nodes_Screen_noInput_background] = "nscr_Nodes_Screen_noInput_background";
	g_ColorNames[EColor::Nodes_Screen_noInput_text] = "nscr_Nodes_Screen_noInput_text";

	// Cycle
	// large button icons
	g_ColorNames[EColor::Cycle_Button] = "ncyc_Button";
	g_ColorNames[EColor::Cycle_ButtonHovered] = "ncyc_ButtonHovered";
	g_ColorNames[EColor::Cycle_ButtonActive] = "ncyc_ButtonActive";
	g_ColorNames[EColor::Cycle_ButtonForeground] = "ncyc_ButtonForeground";
	g_SizeNames[ESize::Cycle_ButtonRounding] = "ncyc_ButtonRounding";
	g_SizeVecNames[ESizeVec2::Cycle_ButtonSize] = "ncyc_ButtonSize";
	// radiobuttons
	g_ColorNames[EColor::Cycle_RadioButton] = "ncyc_RadioButton";
	g_ColorNames[EColor::Cycle_RadioButtonActive] = "ncyc_RadioButtonActive";
	g_ColorNames[EColor::Cycle_RadioButtonHovered] = "ncyc_RadioButtonHovered";
	g_ColorNames[EColor::Cycle_RadioButtonText] = "ncyc_RadioButtonText";
	g_ColorNames[EColor::Cycle_RadioButtonSelected] = "ncyc_RadioButtonSelected";
	g_ColorNames[EColor::Cycle_RadioButtonSelectedHovered] = "ncyc_RadioButtonSelectedHovered";
	g_ColorNames[EColor::Cycle_RadioButtonSelectedText] = "ncyc_RadioButtonSelectedText";
	g_ColorNames[EColor::Cycle_RadioButtonBackground] = "ncyc_RadioButtonBackground";
	g_SizeNames[ESize::Cycle_RadioButtonRounding] = "ncyc_RadioButtonRounding";

	// Links
	g_ColorNames[EColor::Links_selected_colorShift] = "nlnk_selected_colorShift";

	// Global spacing and other size properties.
	g_SizeNames[ESize::Float_inactive_alphaMultiplicator] = "ngen_Float_inactive_alphaMultiplicator";

	g_SizeNames[ESize::Nodes_Operators_Rounding] = "ngen_Nodes Operators Rounding";
	g_SizeNames[ESize::Nodes_Sequence_Rounding] = "ngen_Nodes Sequence Rounding";
	g_SizeNames[ESize::Nodes_LOD_Button_Rounding] = "ngen_Nodes LOD Button Rounding";
	g_SizeNames[ESize::Nodes_Border_Rounding] = "ngen_Nodes Border Rounding";
	g_SizeNames[ESize::Nodes_Border_Thickness] = "ngen_Nodes Border Thickness";

	g_SizeNames[ESize::Nodes_Rounding] = "ngen_Nodes Rounding";
	g_SizeNames[ESize::Nodes_FloatWidth] = "ngen_Nodes Float Width";
	g_SizeNames[ESize::Nodes_FloatMargin] = "ngen_Nodes Float Margin";
	g_SizeNames[ESize::Nodes_BorderWidth] = "ngen_Nodes BorderWidth";

	g_SizeNames[ESize::Nodes_LabelIndent] = "ngen_Pin labels Indent";
	g_SizeNames[ESize::Nodes_HeaderLabelIndent] = "ngen_Header Label Indent";
	g_SizeNames[ESize::Nodes_trackballButtonHeight] = "ngen_Trackball Button Height";
	g_SizeNames[ESize::Nodes_TrackBallSensitivity] = "ngen_Trackball Sensitivity";

	g_SizeNames[ESize::Nodes_FloatInnerPadding] = "ngen_Float Inner Padding";

	g_SizeNames[ESize::Nodes_dragSpeedDefaulrRatio] = "ngen_Drag Speed fo Float";
	g_SizeNames[ESize::Nodes_CtrlMultiplicator] = "ngen_CTRL Multiplicator";
	g_SizeNames[ESize::Nodes_SHIFTMultiplicator] = "ngen_SHIFT Multiplicator";
	g_SizeNames[ESize::Nodes_ALTMultiplicator] = "ngen_ALT Multiplicator";

	g_SizeNames[ESize::Nodes_InputsAlignment] = "ngen_Nodes Inputs Alignment";
	g_SizeNames[ESize::Nodes_MiddleAlignment] = "ngen_Nodes Middle Alignment";
	g_SizeNames[ESize::Nodes_OutputsAlignment] = "ngen_Nodes Outputs Alignment";

	g_SizeNames[ESize::Nodes_leftSideSpacing] = "ngen_Nodes Left Side spacing";
	g_SizeNames[ESize::Nodes_rightSideSpacing] = "ngen_Nodes Right Side spacing";

	g_SizeNames[ESize::Nodes_Transformation_TrackingMarkSize] = "ngen_Nodes Transformation TrackingMarkSize";

	g_SizeNames[ESize::Default_VisiblePrecision] = "ngen_Nodes Default Visible Precision";
	g_SizeNames[ESize::Default_VisibleQuaternionPrecision] = "ngen_Nodes Default Visible Precision For Quaternions";

	g_SizeNames[ESize::Default_InactiveMark] = "ngen_Nodes Default Inactive Part Marker";

	g_SizeNames[ESize::Links_ControlpointsPositionFraction] =
	    "ngen_Link x distance between controlpoint and start/end of link - "
	    "fraction of start to end x distance";
	g_SizeNames[ESize::Links_ControlpointsPositionMin] =
	    "ngen_Link minimal x distance between controlpoint and start/end of link";

	g_SizeNames[ESize::Links_Thickness] = "ngen_Link Thickness";
	g_SizeNames[ESize::Links_ThicknessSelectedBorder] = "ngen_Link Additional thickness when selected";
	g_SizeNames[ESize::Links_selected_alpha] = "ngen_Links Selected Alpha";

	g_SizeNames[ESize::Pins_IconPadding] = "ngen_Pins IconPadding";

	g_SizeVecNames[ESizeVec2::Nodes_Screen_resizeButtonSize] = "ngen_Nodes Screen ResizeButtonSize";
	g_SizeVecNames[ESizeVec2::Nodes_Sequence_DummySpaceSize] = "ngen_Nodes Sequence DummySpaceSize";
	g_SizeVecNames[ESizeVec2::NewNode_positionShift] = "ngen_NewNode PositionShift";


	g_SizeVecNames[ESizeVec2::Nodes_IconSize] = "ngen_Nodes Icon Size";
	g_SizeVecNames[ESizeVec2::Nodes_PivotAlignment] = "ngen_Nodes Pivot Alignment";
	g_SizeVecNames[ESizeVec2::Nodes_PinSpacing] = "ngen_Nodes Pin Spacing";
	g_SizeVecNames[ESizeVec2::Nodes_ItemsSpacing] = "ngen_Nodes Items Spacing";
	g_SizeVecNames[ESizeVec2::Nodes_FloatPadding] = "ngen_Nodes Float Padding";

	g_SizeVecNames[ESizeVec2::Nodes_PivotSize] = "ngen_Nodes Pivot Size";

	g_SizeVecNames[ESizeVec2::Nodes_InputsSize] = "ngen_Nodes Inputs Size";
	g_SizeVecNames[ESizeVec2::Nodes_MiddleSize] = "ngen_Nodes Middle Size";
	g_SizeVecNames[ESizeVec2::Nodes_OutputSize] = "ngen_Nodes Output Size";

	g_SizeVecNames[ESizeVec2::Nodes_LODButtonSize] = "ngen_Nodes LOD Button Size";

	g_SizeVecNames[ESizeVec2::Nodes_FloatCycleButtonSize] = "ngen_Nodes Float Cycle Button Size";
	g_SizeVecNames[ESizeVec2::Nodes_ScreenTextureSize] = "ngen_Nodes Screen Texture Size";

	g_SizeVecNames[ESizeVec2::Builder_ItemSpacing] = "ngen_Builder Item Spacing";
}

void Theme::apply()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = m_colors[EColor::Text];
	style.Colors[ImGuiCol_TextDisabled] = m_colors[EColor::TextDisabled];
	style.Colors[ImGuiCol_WindowBg] = m_colors[EColor::WindowBackground];
	style.Colors[ImGuiCol_PopupBg] = m_colors[EColor::PopupBackground];
	style.Colors[ImGuiCol_Border] = m_colors[EColor::Border];
	style.Colors[ImGuiCol_FrameBg] = m_colors[EColor::FrameBg];
	style.Colors[ImGuiCol_FrameBgHovered] = m_colors[EColor::FrameBgHovered];
	style.Colors[ImGuiCol_FrameBgActive] = m_colors[EColor::FrameBgActive];
	style.Colors[ImGuiCol_TitleBg] = m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_TitleBgActive] = m_colors[EColor::ActiveColor];
	style.Colors[ImGuiCol_TitleBgCollapsed] = m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_MenuBarBg] = m_colors[EColor::MenuBarBackground];
	style.Colors[ImGuiCol_Button] = m_colors[EColor::Button];
	style.Colors[ImGuiCol_ButtonHovered] = m_colors[EColor::ButtonHovered];
	style.Colors[ImGuiCol_ButtonActive] = m_colors[EColor::ButtonActive];
	style.Colors[ImGuiCol_Tab] = m_colors[EColor::Tab];
	style.Colors[ImGuiCol_TabHovered] = m_colors[EColor::TabHovered];
	style.Colors[ImGuiCol_TabActive] = m_colors[EColor::TabActive];

	style.Colors[ImGuiCol_Tab] = m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_TabActive] = m_colors[EColor::SelectionColor];
	style.Colors[ImGuiCol_TabUnfocused] = m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_TabUnfocusedActive] = m_colors[EColor::PrimaryColor];

	style.Colors[ImGuiCol_Separator] = m_colors[EColor::PrimaryColor];

	style.Colors[ImGuiCol_PlotHistogram] = m_colors[EColor::SelectionColor];

	style.FramePadding.x = m_sizesVec2[ESizeVec2::Window_FramePadding].x;
	style.FramePadding.y = m_sizesVec2[ESizeVec2::Window_FramePadding].y;
	style.TabRounding = 2.0f;

	// Show borders.
	style.ChildBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.WindowBorderSize = 1.0f;

	// SS, MH please check this
	//---------------------------------------------------------------------------------------
	/* \todo JH \todo JH repaire
	auto& nodesStyle = ax::NodeEditor::GetStyle();

	nodesStyle.NodeRounding																					 =
	m_sizes[ESize::Nodes_Rounding]; nodesStyle.NodeBorderWidth =
	m_sizes[ESize::Nodes_BorderWidth];
	nodesStyle.Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg] =
	m_colors[EColor::NodeBgOperator];

	nodesStyle.Colors[ax::NodeEditor::StyleColor::StyleColor_Bg]		 =
	m_colors[EColor::NodeEditorBg];
	nodesStyle.Colors[ax::NodeEditor::StyleColor::StyleColor_Grid] 		 =
	m_colors[EColor::NodeEditorBg];
	*/
	//---------------------------------------------------------------------------------------
}

ImFont* Theme::get(EFont font)
{
	return I3T::getUI()->getFonts()[m_fontsAssoc[font]];
}

const char* Theme::getCategoryName(const std::string& key)
{
	auto name = Theme::getCategoryNames()[key];
	assert(name && "Unknown category, did you create entry in g_CategoryNames?");

	return name;
}

Theme::CategoryNames& Theme::getCategoryNames()
{
	return g_CategoryNames;
}

std::map<EColor, const char*>& Theme::getColorNames()
{
	return g_ColorNames;
}

std::map<ESize, const char*>& Theme::getSizeNames()
{
	return g_SizeNames;
}

std::map<ESizeVec2, const char*>& Theme::getSizeVecNames()
{
	return g_SizeVecNames;
}

ThemeGroup& Theme::group(const char* name)
{
	s_variables.emplace_back(name);

	return s_variables.back();
}

bool Detail::isLightThemeSet()
{
#ifdef WIN32
	// based on
	// https://stackoverflow.com/questions/51334674/how-to-detect-windows-10-light-dark-mode-in-win32-application

	// The value is expected to be a REG_DWORD, which is a signed 32-bit little-endian
	auto buffer = std::vector<char>(4);
	auto cbData = static_cast<DWORD>(buffer.size() * sizeof(char));
	auto res = RegGetValueW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
	                        L"AppsUseLightTheme",
	                        RRF_RT_REG_DWORD, // expected value type
	                        nullptr, buffer.data(), &cbData);

	if (res != ERROR_SUCCESS)
	{
		LOG_ERROR("error_code: " + std::to_string(res));
		return false;
	}

	// convert bytes written to our buffer to an int, assuming little-endian
	auto i = int(buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0]);

	return i == 1;
#endif

	return false;
}
