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

#include "API.h"
#include "Config.h"
#include "Utils/HSLColor.h"

static Theme::CategoryNames g_CategoryNames;
static std::map<EColor, const char*> g_ColorNames;
static std::map<ESize, const char*> g_SizeNames;
static std::map<ESizeVec2, const char*> g_SizeVecNames;

Theme Theme::createDefaultModern()
{
	Theme theme;

	theme.initModernProperties();
	theme.initFonts();

	theme.m_name = "Modern";

	return theme;
}

Theme Theme::createDefaultClassic()
{
	Theme theme;

	theme.initClassicProperties();
	theme.initFonts();

	theme.m_name = "Classic";

	return theme;
}

Theme::Theme(std::string name, const Theme::Colors& colors, const Theme::Sizes& sizes, const Theme::SizesVec& sizesVec)
{
	initClassicProperties();

	m_name = std::move(name);
	copyProperties(m_colors, colors);
	copyProperties(m_sizes, sizes);
	copyProperties(m_sizesVec2, sizesVec);
}

void Theme::initFonts()
{
	auto& io = ImGui::GetIO();

	m_fontsAssoc.insert(std::pair(EFont::Regular, "Roboto14"));
	m_fontsAssoc.insert(std::pair(EFont::LargeBold, "RobotoBold20"));

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
	set(EColor::WindowBackground, ImVec4(0.439f, 0.439f, 0.455f, 1.00f));
	set(EColor::PopupBackground, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	set(EColor::MenuBarBackground, ImVec4(0.40f, 0.38f, 0.369f, 1.00f));
	set(EColor::Border, ImVec4(0.278f, 0.278f, 0.286f, 1.00f));
	set(EColor::PrimaryColor, ImVec4(0.278f, 0.278f, 0.286f, 1.00f));
	set(EColor::ActiveColor, ImVec4(0.259f, 0.588f, 0.980f, 1.00f));
	set(EColor::FloatBg, ImVec4(0.16f, 0.29f, 0.48f, 0.54f));
	set(EColor::FloatBgHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.4f));
	set(EColor::FloatBgActive, ImVec4(0.26f, 0.59f, 0.98f, 0.67f));
	set(EColor::AddMenuHeader, ImVec4(0.5f, 0.5f, 0.5f, 1.f));
	set(EColor::SceneViewBackground, ImVec4(0.3f, 0.3f, 0.35f, 1.f));

	auto dockTabActive = HSLColor::fromRGB(0.278f, 0.278f, 0.286f).lighten(0.2f).getRGB();
	set(EColor::DockTabActive, ImVec4(dockTabActive[0], dockTabActive[1], dockTabActive[2], 1.00f));

	set(EColor::Workspace_SelectedBorder, createColor(88, 255, 234, 150));
	set(EColor::Workspace_FocusBorder, createColor(0, 0, 0, 0));
	set(EColor::Workspace_InteractionFocusBorder, createColor(0, 0, 0, 75));
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

	set(EColor::Item_SelectedBorder, createColor(100, 100, 0, 255));

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
	m_sizes[ESize::TutorialWindow_MainMenuButtonWidth] = 80.0f;

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

void Theme::initModernProperties()
{
	set(EColor::Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	set(EColor::WindowBackground, ImVec4(0.439f, 0.439f, 0.455f, 1.00f));
	set(EColor::PopupBackground, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	set(EColor::MenuBarBackground, ImVec4(0.40f, 0.38f, 0.369f, 1.00f));
	set(EColor::Border, ImVec4(0.278f, 0.278f, 0.286f, 1.00f));
	set(EColor::PrimaryColor, ImVec4(0.278f, 0.278f, 0.286f, 1.00f));
	set(EColor::ActiveColor, ImVec4(0.259f, 0.588f, 0.980f, 1.00f));
	set(EColor::FloatBg, ImVec4(0.16f, 0.29f, 0.48f, 0.54f));
	set(EColor::FloatBgHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.4f));
	set(EColor::FloatBgActive, ImVec4(0.26f, 0.59f, 0.98f, 0.67f));
	set(EColor::AddMenuHeader, ImVec4(0.5f, 0.5f, 0.5f, 1.f));
	set(EColor::SceneViewBackground, ImVec4(0.3f, 0.3f, 0.35f, 1.f));

	// Special color for focused docked windows
	auto dockTabActive = HSLColor::fromRGB(0.278f, 0.278f, 0.286f).lighten(0.2f).getRGB();
	set(EColor::DockTabActive, ImVec4(dockTabActive[0], dockTabActive[1], dockTabActive[2], 1.00f));

	set(EColor::Workspace_SelectedBorder, createColor(255, 225, 100, 150));
	set(EColor::Workspace_FocusBorder, createColor(0, 0, 0, 51));
	set(EColor::Workspace_InteractionFocusBorder, createColor(0, 0, 0, 51));
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
	//	set(EColor::Nodes_FloatText, ImVec4(0.0f, 0.0f, 0.0f, 1.00f));
	//	set(EColor::NodeEditorBg, ImVec4(0.298f, 0.298f, 0.298f, 1.00f));
	set(EColor::Nodes_ConnectionPossible, createColor(0, 255, 0, 255));
	set(EColor::Nodes_ConnectionNotPossible, createColor(255, 0, 0, 255));
	set(EColor::Nodes_CreateNode, createColor(32, 45, 32, 180));

	set(EColor::Builder_NodePadding, ImVec4(0, 0, 0, 0));

	// Pin colors
	set(EColor::PulsePin, createColor(164, 58, 190, 255));
	set(EColor::FloatPin, createColor(58, 144, 66, 255));
	set(EColor::MatrixPin, createColor(178, 71, 66, 255));
	set(EColor::QuatPin, createColor(178, 144, 66, 255));
	set(EColor::Vec3Pin, createColor(58, 84, 187, 255));
	set(EColor::Vec4Pin, createColor(106, 96, 67, 255));
	set(EColor::ScreenPin, createColor(51, 150, 215, 255));
	set(EColor::MatrixMulPin, createColor(68, 201, 156, 255));

	// Pin inner colors
	set(EColor::InnerPulsePin, createColor(255, 255, 255, 255));
	set(EColor::InnerFloatPin, createColor(255, 255, 255, 255));
	set(EColor::InnerMatrixPin, createColor(255, 255, 255, 255));
	set(EColor::InnerQuatPin, createColor(255, 255, 255, 255));
	set(EColor::InnerVec3Pin, createColor(255, 255, 255, 255));
	set(EColor::InnerVec4Pin, createColor(255, 255, 255, 255));
	set(EColor::InnerScreenPin, createColor(255, 255, 255, 255));
	set(EColor::InnerMatrixMulPin, createColor(255, 255, 255, 255));

	set(EColor::Item_SelectedBorder, createColor(100, 100, 0, 255));

	// General unspecified node
	set(EColor::NodeBg, createColor(70, 104, 134, 255));
	set(EColor::NodeHeader, createColor(0, 0, 0, 50));
	set(EColor::NodeFont, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

	// Sequence
	set(EColor::NodeBgSequence, ImVec4(0.541f, 0.541f, 0.541f, 1.0f));
	set(EColor::NodeHeaderSequence, ImVec4(0.431f, 0.431f, 0.431f, 1.0f));

	// Operator
	set(EColor::NodeBgOperator, createColor(62, 72, 83, 255));
	set(EColor::NodeHeaderOperator, createColor(55, 55, 55, 255));
	set(EColor::FloatBgOperator, createColor(255, 255, 255, 255));
	set(EColor::FloatBgOperatorActive, ImVec4(97.0f / 255.0f, 105.0f / 255.0f, 126.0f / 255.0f, 1.00f));
	set(EColor::FloatBgOperatorHovered, ImVec4(87.0f / 255.0f, 95.0f / 255.0f, 116.0f / 255.0f, 1.00f));

	// Transform
	set(EColor::NodeBgTransformation, createColor(120, 120, 132, 255));
	set(EColor::NodeHeaderTranformation, createColor(32, 32, 32, 111));
	set(EColor::NodeBorder, ImVec4(0.0f, 0.0f, 0.0f, 0.10f));
	set(EColor::NodeLODButtonColorText, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	set(EColor::NodeLODButtonColor, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
	set(EColor::NodeLODButtonColorActive, ImVec4(0.0f, 0.0f, 0.0f, 0.05f));
	set(EColor::NodeLODButtonColorHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.20f));
	set(EColor::FloatBgTransformation, createColor(255, 255, 255, 255));
	set(EColor::FloatBgTransformationActive, ImVec4(111.0f / 255.0f, 104.0f / 255.0f, 87.0f / 255.0f, 1.00f));
	set(EColor::FloatBgTransformationHovered, createColor(101, 94, 77, 255));

	set(EColor::Links_selected_colorShift, ImVec4(0.2f, 0.2f, 0.2f, 0.0f));

	set(EColor::Synergies_FloatBg, ImVec4(0.0, 0.4f, 0.0f, 1.0f));
	set(EColor::Synergies_FloatBgHovered, ImVec4(0.0f, 0.4f, 0.2f, 1.0f));
	set(EColor::Synergies_FloatBgActive, ImVec4(0.0f, 0.4f, 0.4f, 1.0f));

	set(EColor::Nodes_Screen_resizeBtn_bgShape, ImVec4(0.4f, 0.2f, 0.2f, 0.6f));
	set(EColor::Nodes_Screen_resizeBtn_bgInner, ImVec4(0.4f, 0.2f, 0.2f, 0.6f));
	set(EColor::Nodes_Screen_resizeBtn_fgShape, ImVec4(0.4f, 0.4f, 0.6f, 0.6f));
	set(EColor::Nodes_Screen_resizeBtn_fgInner, ImVec4(0.4f, 0.4f, 0.6f, 0.6f));
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

	// Cycle

	// set(EColor::Cycle_Button, createColor(120, 120, 120, 255));
	// set(EColor::Cycle_ButtonHovered, createColor(135, 135, 135, 255));
	// set(EColor::Cycle_ButtonActive, createColor(150, 150, 150, 255));
	// set(EColor::Cycle_ButtonForeground, createColor(0, 0, 0, 255));

	// set(EColor::Cycle_RadioButton, createColor(33, 33, 33, 255));
	// set(EColor::Cycle_RadioButtonActive, createColor(158, 220, 213, 255));
	// set(EColor::Cycle_RadioButtonHovered, createColor(37, 37, 337, 255));
	// set(EColor::Cycle_RadioButtonText, createColor(89, 89, 89, 255));
	// set(EColor::Cycle_RadioButtonSelected, createColor(160, 220, 200, 255));
	// set(EColor::Cycle_RadioButtonSelectedHovered, createColor(180, 240, 220, 255));
	// set(EColor::Cycle_RadioButtonSelectedText, createColor(0, 0, 0, 255));
	// set(EColor::Cycle_RadioButtonBackground, createColor(33, 33, 33, 255));

	set(EColor::Cycle_Button, createColor(120, 120, 120, 255));
	set(EColor::Cycle_ButtonHovered, createColor(140, 135, 135, 255)); //
	set(EColor::Cycle_ButtonActive, createColor(130, 130, 130, 255));  //
	set(EColor::Cycle_ButtonForeground, createColor(0, 0, 0, 255));

	set(EColor::Cycle_RadioButton, createColor(33, 33, 33, 255));
	set(EColor::Cycle_RadioButtonHovered, createColor(55, 55, 55, 255));
	set(EColor::Cycle_RadioButtonActive, createColor(85, 85, 85, 255));
	set(EColor::Cycle_RadioButtonText, createColor(89, 89, 89, 255));
	set(EColor::Cycle_RadioButtonSelected, createColor(145, 198, 201, 219));
	set(EColor::Cycle_RadioButtonSelectedHovered, createColor(150, 200, 205, 230));
	set(EColor::Cycle_RadioButtonSelectedText, createColor(0, 0, 0, 255)); //
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
	m_sizes[ESize::TutorialWindow_MainMenuButtonWidth] = 70.0f;

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
	// All category keys must be I3T_PROPERTY_NAME_OFFSET characters long.
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
	g_ColorNames[EColor::Text] = "glob_Text";
	g_ColorNames[EColor::Border] = "glob_Border";
	g_ColorNames[EColor::WindowBackground] = "glob_Window Background";
	g_ColorNames[EColor::PopupBackground] = "glob_Popup Background";
	g_ColorNames[EColor::MenuBarBackground] = "glob_Menu Bar Background";
	g_ColorNames[EColor::PrimaryColor] = "glob_Primary Color (tabs, tiles, ...)";
	g_ColorNames[EColor::ActiveColor] = "glob_Active Color";
	g_ColorNames[EColor::TabColor] = "glob_Tab Color";
	g_ColorNames[EColor::SceneViewBackground] = "glob_Tab SceneView Background";
	g_ColorNames[EColor::DockTabActive] = "glob_Dock Active Color";
	g_ColorNames[EColor::SelectionRectFull] = "glob_Selection rectangle full";
	g_ColorNames[EColor::SelectionRectTouch] = "glob_Selection rectangle touch";
	g_ColorNames[EColor::Workspace_SelectedBorder] = "glob_Selected node border";

	g_SizeNames[ESize::Workspace_SelectedBorderThickness] = "glob_Selected node border thickness";
	g_SizeVecNames[ESizeVec2::Window_FramePadding] = "glob_Windows Frame Padding";

	// Tutorials
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

	g_SizeVecNames[ESizeVec2::StartWindow_WinSize] = "star_Window Size";
	g_SizeVecNames[ESizeVec2::StartWindow_LogoOffset] = "star_Logo Offset";
	g_SizeVecNames[ESizeVec2::StartWindow_WinPadding] = "star_Window Padding";
	g_SizeVecNames[ESizeVec2::StartWindow_LeftWinPadding] = "star_Left Window Padding";
	g_SizeVecNames[ESizeVec2::StartWindow_RightWinOuterPadding] = "star_Right Window OuterPadding";
	g_SizeVecNames[ESizeVec2::StartWindow_RightWinInnerPadding] = "star_Right Window InnerPadding";

	// About Window
	g_ColorNames[EColor::AboutWindow_BackgroundLeft] = "abut_AboutWindow BackgroundLeft";
	g_ColorNames[EColor::AboutWindow_BackgroundRight] = "abut_AboutWindow BackgroundRight";
	g_ColorNames[EColor::AboutWindow_Text] = "abut_AboutWindow Text";

	// Node editor colors
	// ------------------

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
	g_ColorNames[EColor::Item_SelectedBorder] = "ngen_Item Selected Border";
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

	style.Colors[ImGuiCol_WindowBg] = m_colors[EColor::WindowBackground];
	style.Colors[ImGuiCol_Border] = m_colors[EColor::Border];
	style.Colors[ImGuiCol_PopupBg] = m_colors[EColor::PopupBackground];
	style.Colors[ImGuiCol_TitleBg] = m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_TitleBgActive] = m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_TitleBgCollapsed] = m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_MenuBarBg] = m_colors[EColor::MenuBarBackground];

	style.Colors[ImGuiCol_Tab] = m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_TabActive] = m_colors[EColor::ActiveColor];
	style.Colors[ImGuiCol_TabUnfocused] = m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_TabUnfocusedActive] = m_colors[EColor::PrimaryColor];

	style.Colors[ImGuiCol_Separator] = m_colors[EColor::PrimaryColor];

	style.Colors[ImGuiCol_PlotHistogram] = m_colors[EColor::ActiveColor];

	style.FramePadding.x = m_sizesVec2[ESizeVec2::Window_FramePadding].x;
	style.FramePadding.y = m_sizesVec2[ESizeVec2::Window_FramePadding].y;
	style.TabRounding = 2.0f;

	// Show borders.
	style.ChildBorderSize = 0.0f;
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

// JH unused
// void Theme::operatorColorTheme()
//{
//    /* \todo JH repaire
//	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg]
//= m_colors[EColor::NodeBgOperator];
//
//	ImGuiStyle& style = ImGui::GetStyle();
//
//	style.Colors[ImGuiCol_FrameBg]				= m_colors[EColor::FloatBgOperator];
//	style.Colors[ImGuiCol_FrameBgHovered] =
// m_colors[EColor::FloatBgOperatorHovered];
// style.Colors[ImGuiCol_FrameBgActive] =
// m_colors[EColor::FloatBgOperatorActive];
//	*/
//}
//
// void Theme::transformationColorTheme()
//{
//    /* \todo JH repaire
//	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg]
//= 			m_colors[EColor::NodeBgTransformation];
//
//	ImGuiStyle& style = ImGui::GetStyle();
//
//	style.Colors[ImGuiCol_FrameBg]				=
// m_colors[EColor::FloatBgTransformation];
// style.Colors[ImGuiCol_FrameBgHovered] =
// m_colors[EColor::FloatBgTransformationHovered];
//	style.Colors[ImGuiCol_FrameBgActive]	=
// m_colors[EColor::FloatBgTransformationActive];
//	*/
//}

bool isVectorsEqual(ImVec4 a, ImVec4 b)
{
	/// \todo Comparing float values!
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w ? true : false;
}

// JH unused
// ImVec4 Theme::getHeader()
//{
//    /* \todo JH repaire
//	if
//(isVectorsEqual(ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg],
//										 m_colors[EColor::NodeBgOperator]))
//	{
//		return m_colors[EColor::NodeHeaderOperator];
//	}
//	if
//(isVectorsEqual(ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg],
//										 m_colors[EColor::NodeBgTransformation]))
//	{
//		return m_colors[EColor::NodeHeaderTranformation];
//	}
//	*/
//	return m_colors[EColor::NodeHeaderTranformation];
//	//return ImVec4(0, 0, 0, 0);
//}
//
// ImVec4 Theme::getBg()
//{
//    /* \todo JH repaire
//	if
//(isVectorsEqual(ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg],
//										 m_colors[EColor::NodeBgOperator]))
//	{
//		return m_colors[EColor::NodeHeaderOperator];
//	}
//	if
//(isVectorsEqual(ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg],
//										 m_colors[EColor::NodeBgTransformation]))
//	{
//		return m_colors[EColor::NodeHeaderTranformation];
//	}
//	*/
//	return m_colors[EColor::NodeBgTransformation];
//	//return ImVec4(0, 0, 0, 0);
//}

void Theme::returnFloatColorToDefault()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_FrameBg] = m_colors[EColor::FloatBg];
	style.Colors[ImGuiCol_FrameBgHovered] = m_colors[EColor::FloatBgHovered];
	style.Colors[ImGuiCol_FrameBgActive] = m_colors[EColor::FloatBgHovered];
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
