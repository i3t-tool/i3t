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

#include "Config.h"
#include "GUI/Workspace/WorkspaceModule.h"
#include "I3T.h"

// TODO: (DR) Why are these maps? Enums are unique int ids we can just use a vector
static std::map<EColor, std::string> g_ColorNames;
static std::map<ESize, std::string> g_SizeNames;
static std::map<ESizeVec2, std::string> g_SizeVecNames;

DIWNE::StyleOverride Theme::m_nodeStyle;
DIWNE::StyleOverride Theme::m_transformationStyle;
DIWNE::StyleOverride Theme::m_operatorStyle;

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
			LOG_WARN("[THEME] Style var {}::{} is not registered!.", magic_enum::enum_type_name<T>(),
			         magic_enum::enum_name(enumValue));
			assert(false && "An enum key was not added to any ThemeGroup! See Theme::initNames().");
		}
	}
}

static void initializeAndValidateThemeVariables()
{
	LOG_DEBUG("[THEME] Initializing theme variables");

	std::set<EColor> colors;
	std::set<ESize> sizes;
	std::set<ESizeVec2> sizeVecs;

	for (const auto& group : Theme::getVariables())
	{
		for (const auto& var : group.variables)
		{
			std::string varKeyName = std::string(group.id) + "_" + var.name;
			std::visit(Detail::Overloaded{[&](EColor color) {
				                              colors.insert(color);
				                              g_ColorNames[color] = std::move(varKeyName);
			                              },
			                              [&](ESize size) {
				                              sizes.insert(size);
				                              g_SizeNames[size] = std::move(varKeyName);
			                              },
			                              [&](ESizeVec2 sizeVec) {
				                              sizeVecs.insert(sizeVec);
				                              g_SizeVecNames[sizeVec] = std::move(varKeyName);
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

	theme.initDefaultClassic();
	theme.initFonts();

	theme.m_name = I3T_CLASSIC_THEME_NAME;

	return theme;
}

Theme::Theme(std::string name, bool isDark, const Theme::Colors& colors, const Theme::Sizes& sizes,
             const Theme::SizesVec& sizesVec)
{
	initDefaultClassic();

	m_name = std::move(name);
	m_isDark = isDark;
	copyColors(m_colors, colors);
	// We do not serialize the dpi scaling boolean right now
	// So we infer the dpi switch from the default values during copying
	copySizes(m_sizes, sizes);
	copySizes(m_sizesVec2, sizesVec);
}

void Theme::initFonts()
{
	auto& io = ImGui::GetIO();

	// TODO: (DR) I still feel like we have too many specific fonts.
	//  Fonts should have as many uses as possible with the possibility of scaling them down for particular use cases.
	//  It is not a good idea to load and rasterize whole new font just because it's used somewhere once
	//  Hence I would prefer if the tutorial / welcome window specific fonts were reduced and reused elsewhere

	// Generic fonts
	m_fontsAssoc.insert(std::pair(EFont::Regular, "Roboto14"));
	m_fontsAssoc.insert({EFont::Mono, "RobotoMono14"});
	m_fontsAssoc.insert(std::pair(EFont::Button, "RobotoBold12"));

	// Tutorial fonts.
	m_fontsAssoc.insert(std::pair(EFont::TutorialTitle, "UbuntuBold24"));
	m_fontsAssoc.insert(std::pair(EFont::TutorialText, "Roboto16"));
	m_fontsAssoc.insert(std::pair(EFont::TutorialBold, "RobotoBold16"));
	m_fontsAssoc.insert(std::pair(EFont::TutorialHint, "RobotoItalic16"));

	// Welcome window fonts
	m_fontsAssoc.insert(std::pair(EFont::WelcomeTitle, "UbuntuBold33.5"));
	m_fontsAssoc.insert(std::pair(EFont::WelcomeItemTitle, "UbuntuBold18"));

	// Set default ImGui font
	I3T::getUI()->getFontManager().setDefaultFont(m_fontsAssoc[EFont::Regular]);
}

void Theme::initDefaultClassic()
{
	// ImGui dark style
	set(EColor::Text, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
	set(EColor::TextDisabled, ImVec4(0.50f, 0.50f, 0.50f, 1.00f));
	set(EColor::WindowBackground, ImVec4(0.06f, 0.06f, 0.06f, 0.94f));
	set(EColor::ChildBackground, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
	set(EColor::PopupBackground, ImVec4(0.08f, 0.08f, 0.08f, 0.94f));
	set(EColor::Border, ImVec4(0.43f, 0.43f, 0.50f, 0.50f));
	set(EColor::BorderShadow, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
	set(EColor::BorderDim, ImVec4(0.1f, 0.1f, 0.1f, 0.50f));
	set(EColor::FrameBg, ImVec4(0.16f, 0.29f, 0.48f, 0.54f));
	set(EColor::FrameBgHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	set(EColor::FrameBgActive, ImVec4(0.26f, 0.59f, 0.98f, 0.67f));
	set(EColor::WindowTitleBg, ImVec4(0.04f, 0.04f, 0.04f, 1.00f));
	set(EColor::WindowTitleBgActive, ImVec4(0.16f, 0.29f, 0.48f, 1.00f));
	set(EColor::WindowTitleBgCollapsed, ImVec4(0.00f, 0.00f, 0.00f, 0.51f));
	set(EColor::MenuBarBackground, ImVec4(0.14f, 0.14f, 0.14f, 1.00f));
	set(EColor::Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	set(EColor::ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.00f));
	set(EColor::ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.00f));
	set(EColor::Tab, ImVec4(0.1472f, 0.2608f, 0.4408f, 0.872f));
	set(EColor::TabHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.80f));
	set(EColor::TabActive, ImVec4(0.2008f, 0.4726f, 0.8868f, 1.00f));
	set(EColor::ScrollbarBg, ImVec4(0.02f, 0.02f, 0.02f, 0.53f));
	set(EColor::ScrollbarGrab, ImVec4(0.31f, 0.31f, 0.31f, 1.00f));
	set(EColor::ScrollbarGrabHovered, ImVec4(0.41f, 0.41f, 0.41f, 1.00f));
	set(EColor::ScrollbarGrabActive, ImVec4(0.51f, 0.51f, 0.51f, 1.00f));
	set(EColor::CheckMark, ImVec4(0.26f, 0.59f, 0.98f, 1.00f));
	set(EColor::SliderGrab, ImVec4(0.24f, 0.52f, 0.88f, 1.00f));
	set(EColor::SliderGrabActive, ImVec4(0.26f, 0.59f, 0.98f, 1.00f));
	set(EColor::Header, ImVec4(0.26f, 0.59f, 0.98f, 0.31f));
	set(EColor::HeaderHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.80f));
	set(EColor::HeaderActive, ImVec4(0.26f, 0.59f, 0.98f, 1.00f));
	set(EColor::Separator, ImVec4(0.43f, 0.43f, 0.50f, 0.50f));
	set(EColor::SeparatorHovered, ImVec4(0.10f, 0.40f, 0.75f, 0.78f));
	set(EColor::SeparatorActive, ImVec4(0.10f, 0.40f, 0.75f, 1.00f));
	set(EColor::ResizeGrip, ImVec4(0.26f, 0.59f, 0.98f, 0.20f));
	set(EColor::ResizeGripHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.67f));
	set(EColor::ResizeGripActive, ImVec4(0.26f, 0.59f, 0.98f, 0.95f));
	set(EColor::TabUnfocused, ImVec4(0.094f, 0.2128f, 0.352f, 0.872f));
	set(EColor::TabUnfocusedActive, ImVec4(0.1365f, 0.3696f, 0.6632f, 1.00f));
	set(EColor::DockingPreview, ImVec4(0.26f, 0.59f, 0.98f, 0.70f));
	set(EColor::DockingEmptyBg, ImVec4(0.20f, 0.20f, 0.20f, 1.00f));
	set(EColor::PlotLines, ImVec4(0.61f, 0.61f, 0.61f, 1.00f));
	set(EColor::PlotLinesHovered, ImVec4(1.00f, 0.43f, 0.35f, 1.00f));
	set(EColor::PlotHistogram, ImVec4(0.90f, 0.70f, 0.00f, 1.00f));
	set(EColor::PlotHistogramHovered, ImVec4(1.00f, 0.60f, 0.00f, 1.00f));
	set(EColor::TableHeaderBg, ImVec4(0.19f, 0.19f, 0.20f, 1.00f));
	set(EColor::TableBorderStrong, ImVec4(0.31f, 0.31f, 0.35f, 1.00f));
	set(EColor::TableBorderLight, ImVec4(0.23f, 0.23f, 0.25f, 1.00f));
	set(EColor::TableRowBg, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
	set(EColor::TableRowBgAlt, ImVec4(1.00f, 1.00f, 1.00f, 0.06f));
	set(EColor::TextSelectedBg, ImVec4(0.26f, 0.59f, 0.98f, 0.35f));
	set(EColor::DragDropTarget, ImVec4(1.00f, 1.00f, 0.00f, 0.90f));
	set(EColor::NavHighlight, ImVec4(0.26f, 0.59f, 0.98f, 1.00f));
	set(EColor::NavWindowingHighlight, ImVec4(1.00f, 1.00f, 1.00f, 0.70f));
	set(EColor::NavWindowingDimBg, ImVec4(0.80f, 0.80f, 0.80f, 0.20f));
	set(EColor::ModalWindowDimBg, ImVec4(0.80f, 0.80f, 0.80f, 0.35f));

	// Custom stuff below
	set(EColor::ButtonDim, ImVec4(0.259f, 0.588f, 0.980f, 1.00f));
	set(EColor::ButtonDimHovered, ImVec4(0.259f, 0.588f, 0.980f, 1.00f));
	set(EColor::ButtonDimActive, ImVec4(0.259f, 0.588f, 0.980f, 1.00f));
	set(EColor::ButtonDimToggled, ImVec4(0.259f, 0.588f, 0.980f, 1.00f));
	set(EColor::ButtonDimDark, ImVec4(0.1f, 0.1f, 0.1f, 0.5f));

	set(EColor::SceneViewBackground, ImVec4(0.3f, 0.3f, 0.35f, 1.f));

	set(EColor::DockTab, ImVec4(0.309f, 0.309f, 0.318f, 1.f));
	set(EColor::DockTabActive, ImVec4(0.258f, 0.334f, 0.427f, 1.f));
	set(EColor::DockTabUnfocused, ImVec4(0.278f, 0.278f, 0.286f, 1.00f));
	set(EColor::DockTabUnfocusedActive, ImVec4(0.263f, 0.291f, 0.325f, 1.f));
	set(EColor::DockTabHovered, ImVec4(0.259f, 0.588f, 0.980f, 1.00f));

	set(EColor::FloatBg, ImVec4(0.16f, 0.29f, 0.48f, 0.54f));
	set(EColor::FloatBgHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.4f));
	set(EColor::FloatBgActive, ImVec4(0.26f, 0.59f, 0.98f, 0.67f));

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

	set(EColor::SelectionRectFull, createColor(0, 0, 255, 26));
	set(EColor::SelectionRectTouch, createColor(0, 255, 0, 26));

	// Node Editor
	set(EColor::NodeEditorBackground, ImVec4(0.439f, 0.439f, 0.455f, 1.00f));
	set(EColor::NodeEditorGridColor, ImColor(57, 57, 57, 120));
	set(EColor::NodeEditorGridDotsColor, ImColor(200, 200, 200, 40));
	//	set(EColor::Nodes_FloatText, ImVec4(255.0f / 255.0f, 255.0f / 255.0f,
	// 255.0f / 255.0f, 1.00f)); 	set(EColor::NodeEditorBg, ImVec4(112.0f /
	// 255.0f, 112.0f / 255.0f, 116.0f / 255.0f, 1.00f));
	set(EColor::Nodes_ConnectionPossible, createColor(0, 255, 0, 255));
	set(EColor::Nodes_ConnectionNotPossible, createColor(255, 0, 0, 255));
	set(EColor::Nodes_CreateNode, createColor(32, 45, 32, 180));

	set(EColor::Workspace_SelectedBorder, createColor(88, 255, 234, 150)); // TODO: Missing in DarkTheme
	set(EColor::Workspace_HoverBorder, createColor(0, 0, 0, 50));          // TODO: Missing name!

	set(EColor::DisabledPinColor, createColor(93, 93, 93, 85));

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

	// Folta operator color set
	set(EColor::NodeBgOperator, ImVec4(83.0f / 255.0f, 101.0f / 255.0f, 146.0f / 255.0f, 0.784f));
	set(EColor::NodeHeaderOperator, ImVec4(73.0f / 255.0f, 91.0f / 255.0f, 136.0f / 255.0f, 1.00f));

	// Folta transformation color set
	set(EColor::NodeBgTransformation, ImVec4(137.0f / 255.0f, 115.0f / 255.0f, 59.0f / 255.0f, 0.784f));
	set(EColor::NodeHeaderTranformation, ImVec4(0.498f, 0.412f, 0.192f, 1.00f));
	set(EColor::NodeBorder, ImVec4(0.0f, 0.0f, 0.0f, 0.10f));
	set(EColor::NodeLODButtonColorText, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	set(EColor::NodeContextButtonColorText, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
	set(EColor::NodeLODButtonColor, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
	set(EColor::NodeLODButtonColorActive, ImVec4(0.0f, 0.0f, 0.0f, 0.05f));
	set(EColor::NodeLODButtonColorHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.20f));

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

	set(EColor::Nodes_Tracking_ColorActive, ImVec4(0.7f, 0.7f, 0.0f, 1.0f));
	set(EColor::Nodes_Tracking_ColorInactive, ImVec4(0.7f, 0.7f, 0.0f, 1.0f));
	set(EColor::Nodes_Tracking_Cursor, ImVec4(0.7f, 0.7f, 0.0f, 1.0f));
	set(EColor::Nodes_Tracking_OverlayActive, ImVec4(0.3f, 0.3f, 0.1f, 0.6f));
	set(EColor::Nodes_Tracking_OverlayInactive, ImVec4(0.16f, 0.16f, 0.16f, 0.7f));

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
	set(EColor::Cycle_RadioButtonHovered, createColor(37, 37, 37, 255));
	set(EColor::Cycle_RadioButtonText, createColor(89, 89, 89, 255));
	set(EColor::Cycle_RadioButtonSelected, createColor(140, 190, 186, 255));
	set(EColor::Cycle_RadioButtonSelectedHovered, createColor(145, 195, 186, 255));
	set(EColor::Cycle_RadioButtonSelectedText, createColor(0, 0, 0, 255));
	set(EColor::Cycle_RadioButtonBackground, createColor(33, 33, 33, 255));

	m_sizes[ESize::FloatingButtonRounding] = {2.0f, true};

	m_sizes[ESize::Nodes_Rounding] = {5.0f, true};
	m_sizes[ESize::Nodes_BorderWidth] = {0.0f, true};
	m_sizes[ESize::Nodes_LabelIndent] = {3.0f, true};
	m_sizes[ESize::Nodes_HeaderLabelIndent] = {2.0f, true};
	m_sizes[ESize::Nodes_trackballButtonHeight] = {20.0f, true};
	m_sizes[ESize::Nodes_TrackBallSensitivity] = {5.0f, true};

	m_sizes[ESize::Nodes_FloatInnerPadding] = {1.0f, true};
	m_sizes[ESize::Nodes_FloatCharacterWidthMultiplier] = {0.4f, false};
	m_sizes[ESize::Nodes_FloatMinCharacters] = {5.0f, false};

	m_sizes[ESize::Nodes_dragSpeedDefaultRatio] = {0.015f, true};
	m_sizes[ESize::Nodes_CtrlMultiplicator] = {0.1f, true};
	m_sizes[ESize::Nodes_SHIFTMultiplicator] = {10.0f, true};
	m_sizes[ESize::Nodes_ALTMultiplicator] = {0.01f, true};

	m_sizes[ESize::Nodes_InputsAlignment] = {0.0f, false};
	m_sizes[ESize::Nodes_MiddleAlignment] = {0.0f, false};
	m_sizes[ESize::Nodes_OutputsAlignment] = {0.0f, false};

	m_sizes[ESize::Nodes_leftSideSpacing] = {3.0f, true};
	m_sizes[ESize::Nodes_rightSideSpacing] = {3.0f, true};

	m_sizes[ESize::Workspace_SelectedBorderThickness] = {2.5f, true};
	m_sizes[ESize::Workspace_HoverBorderThickness] = {1.5f, true};

	m_sizes[ESize::Tutorial_TaskSquareXPadding] = {10.0f, true};
	m_sizes[ESize::Tutorial_FrameRounding] = {5.0f, true};
	m_sizes[ESize::Tutorial_ScrollbarRounding] = {5.0f, true};
	m_sizes[ESize::Tutorial_ScrollbarSize] = {15.0f, true};
	m_sizes[ESize::Tutorial_BackButtonWidth] = {40.0f, true};
	m_sizes[ESize::Tutorial_MainMenuButtonWidth] = {120.0f, true};

	m_sizes[ESize::Default_VisiblePrecision] = {1.0f, false};
	m_sizes[ESize::Default_VisibleQuaternionPrecision] = {4.0f, false};

	m_sizes[ESize::Default_InactiveMark] = {0.0f, false};

	m_sizes[ESize::Links_ControlpointsPositionFraction] = {0.2f, false};
	m_sizes[ESize::Links_ControlpointsPositionMin] = {65.0f, true};
	m_sizes[ESize::Links_ControlpointsPositionMax] = {350.0f, true};
	m_sizes[ESize::Links_Thickness] = {5.0f, true};
	m_sizes[ESize::Links_ThicknessSelected] = {7.0f, true};

	m_sizes[ESize::Pins_IconPadding] = {2.0f, true};
	m_sizes[ESize::Links_selected_alpha] = {0.8f, false};

	m_sizes[ESize::Float_inactive_alphaMultiplicator] = {0.5f, false};

	m_sizes[ESize::Nodes_Operators_Rounding] = {5.0f, true};
	m_sizes[ESize::Nodes_Sequence_Rounding] = {5.0f, true};
	m_sizes[ESize::Nodes_LOD_Button_Rounding] = {5.0f, true};
	m_sizes[ESize::Nodes_Border_Rounding] = {5.0f, true};
	m_sizes[ESize::Nodes_Border_Thickness] = {1.5f, true};

	m_sizes[ESize::Nodes_Transformation_TrackingMarkSize] = {5.f, true};

	m_sizes[ESize::Window_Rounding] = {0.0f, true};
	m_sizes[ESize::Frame_Rounding] = {0.0f, true};
	m_sizes[ESize::Tooltip_Rounding] = {10.0f, true};

	m_sizes[ESize::StartWindow_WinWidth] = {850.0f, true};
	m_sizes[ESize::StartWindow_WinHeight] = {500.0f, true};
	m_sizes[ESize::StartWindow_WinRounding] = {6.0f, true};
	m_sizes[ESize::StartWindow_TitleVerticalOffset] = {130.0f, true};
	m_sizes[ESize::StartWindow_LeftBarWidth] = {330.0f, true};
	m_sizes[ESize::StartWindow_LoadButtonWidth] = {120.0f, true};
	m_sizes[ESize::StartWindow_StartNewButtonWidth] = {120.0f, true};
	m_sizes[ESize::StartWindow_ButtonHeight] = {30.0f, true};
	m_sizes[ESize::StartWindow_ThumbImageSize] = {80.0f, true};
	m_sizes[ESize::StartWindow_StartButtonWidth] = {120.0f, true};
	m_sizes[ESize::StartWindow_FrameRounding] = {4.0f, true};
	m_sizes[ESize::StartWindow_ScrollbarSize] = {14.0f, true};
	m_sizes[ESize::StartWindow_YourSceneWinRounding] = {6.0f, true};
	m_sizes[ESize::StartWindow_DotSize] = {10.0f, true};
	m_sizes[ESize::StartWindow_DotSpacing] = {7.0f, true};

	m_sizes[ESize::Cycle_ButtonRounding] = {3.0f, true};
	m_sizes[ESize::Cycle_RadioButtonRounding] = {5.0f, true};

	m_sizesVec2[ESizeVec2::FramePadding] = {{4.0f, 4.0f}, true};
	m_sizesVec2[ESizeVec2::Tooltip_Padding] = {{10.f, 10.f}, true};
	m_sizesVec2[ESizeVec2::Window_Padding] = {{8.f, 8.f}, true};

	m_sizesVec2[ESizeVec2::Nodes_ItemsSpacing] = {{2.0f, 3.0f}, true};
	m_sizesVec2[ESizeVec2::Nodes_FloatPadding] = {{0.0f, 1.0f}, true};
	m_sizesVec2[ESizeVec2::Nodes_PinSpacing] = {{0.0f, 0.0f}, true};
	m_sizesVec2[ESizeVec2::Nodes_PivotAlignment] = {{0.0f, 0.5f}, false};
	m_sizesVec2[ESizeVec2::Nodes_PivotSize] = {{0.0f, 0.0f}, true};

	m_sizesVec2[ESizeVec2::Nodes_InputsSize] = {{0.0f, 0.0f}, true};
	m_sizesVec2[ESizeVec2::Nodes_MiddleSize] = {{0.0f, 0.0f}, true};
	m_sizesVec2[ESizeVec2::Nodes_OutputSize] = {{0.0f, 0.0f}, true};

	m_sizesVec2[ESizeVec2::Nodes_LODButtonSize] = {{25.0f, 25.0f}, true};

	m_sizesVec2[ESizeVec2::Nodes_PinSize] = {{15.0f, 15.0f}, true};
	m_sizesVec2[ESizeVec2::Nodes_PinSize_MatrixMul] = {{16.0f, 16.0f}, true};
	m_sizesVec2[ESizeVec2::Nodes_FloatCycleButtonSize] = {{32.0f, 32.0f}, true};
	m_sizesVec2[ESizeVec2::Nodes_ScreenTextureSize] = {{130.0f, 130.0f}, true};

	m_sizesVec2[ESizeVec2::Builder_ItemSpacing] = {{0.0f, 0.0f}, true};

	m_sizesVec2[ESizeVec2::Nodes_Screen_resizeButtonSize] = {{20.f, 20.f}, true};

	m_sizesVec2[ESizeVec2::Nodes_Sequence_DummySpaceSize] = {{100.f, 1.f}, true};

	// m_sizesVec2[ESizeVec2::Nodes_noPinsSpacing] = {{0.f, 20.f}, true};

	m_sizesVec2[ESizeVec2::Nodes_NewPositionShift] = {{25.f, 25.f}, true};

	m_sizesVec2[ESizeVec2::Tutorial_WindowPadding] = {{30.f, 35.f}, true};

	m_sizesVec2[ESizeVec2::StartWindow_WinSize] = {{1020.f, 800.f}, false};
	m_sizesVec2[ESizeVec2::StartWindow_LogoOffset] = {{5.f, -20.f}, true};
	m_sizesVec2[ESizeVec2::StartWindow_WinPadding] = {{0.f, 0.f}, true};
	m_sizesVec2[ESizeVec2::StartWindow_LeftWinPadding] = {{30.f, 30.f}, true};
	m_sizesVec2[ESizeVec2::StartWindow_RightWinOuterPadding] = {{10.f, 10.f}, true};
	m_sizesVec2[ESizeVec2::StartWindow_RightWinInnerPadding] = {{10.f, 10.f}, true};

	m_sizesVec2[ESizeVec2::Cycle_ButtonSize] = {{40.0f, 40.0f}, true};
}

void Theme::initNames()
{
	group("Global", "glob")
	    .add(EColor::Text, "Text")
	    .add(EColor::TextDisabled, "Disabled Text")
	    .add(EColor::WindowBackground, "Window Background")
	    .add(EColor::ChildBackground, "Child Window Background")
	    .add(EColor::PopupBackground, "Popup Background")
	    .add(EColor::Border, "Border", "Border color of items and windows.")
	    .add(EColor::BorderShadow, "Border Shadow")
	    .add(EColor::BorderDim, "Border Dim", "Dim variant of the border used in some special cases.")
	    .add(EColor::FrameBg, "Frame Background")
	    .add(EColor::FrameBgHovered, "Frame Background Hovered")
	    .add(EColor::FrameBgActive, "Frame Background Active")
	    .add(EColor::WindowTitleBg, "Window Title Background")
	    .add(EColor::WindowTitleBgActive, "Window Title Background Active")
	    .add(EColor::WindowTitleBgCollapsed, "Window Title Background Collapsed")
	    .add(EColor::MenuBarBackground, "Menu Bar Background")
	    .add(EColor::ScrollbarBg, "Scrollbar Background")
	    .add(EColor::ScrollbarGrab, "Scrollbar Grab")
	    .add(EColor::ScrollbarGrabHovered, "Scrollbar Grab Hovered")
	    .add(EColor::ScrollbarGrabActive, "Scrollbar Grab Active")
	    .add(EColor::CheckMark, "Checkmark Color")
	    .add(EColor::SliderGrab, "Slider Grab")
	    .add(EColor::SliderGrabActive, "Slider Grab Active")
	    .add(EColor::Button, "Button")
	    .add(EColor::ButtonHovered, "Button Hovered")
	    .add(EColor::ButtonActive, "Button Active")
	    .add(EColor::Header, "Header")
	    .add(EColor::HeaderHovered, "Header Hovered")
	    .add(EColor::HeaderActive, "Header Active")
	    .add(EColor::SliderGrab, "Slider Grab")
	    .add(EColor::SliderGrabActive, "Slider Grab Active")
	    .add(EColor::Button, "Button")
	    .add(EColor::ButtonHovered, "Button Hovered")
	    .add(EColor::ButtonActive, "Button Active")
	    .add(EColor::Header, "Header", "Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem")
	    .add(EColor::HeaderHovered, "Header Hovered")
	    .add(EColor::HeaderActive, "Header Active")
	    .add(EColor::Separator, "Separator")
	    .add(EColor::SeparatorHovered, "Separator Hovered")
	    .add(EColor::SeparatorActive, "Separator Active")
	    .add(EColor::ResizeGrip, "Resize Grip", "Resize grip in lower-right and lower-left corners of windows.")
	    .add(EColor::ResizeGripHovered, "Resize Grip Hovered")
	    .add(EColor::ResizeGripActive, "Resize Grip Active")
	    .add(EColor::Tab, "Tab")
	    .add(EColor::TabHovered, "Tab Hovered")
	    .add(EColor::TabActive, "Tab Active")
	    .add(EColor::TabUnfocused, "Tab Unfocused")
	    .add(EColor::TabUnfocusedActive, "Tab Unfocused Active")
	    .add(EColor::DockingPreview, "Docking Preview")
	    .add(EColor::DockingEmptyBg, "Docking Empty Background")
	    .add(EColor::PlotLines, "Plot Lines")
	    .add(EColor::PlotLinesHovered, "Plot Lines Hovered")
	    .add(EColor::PlotHistogram, "Plot Histogram")
	    .add(EColor::PlotHistogramHovered, "Plot Histogram Hovered")
	    .add(EColor::TableHeaderBg, "Table Header Background")
	    .add(EColor::TableBorderStrong, "Table Border Strong")
	    .add(EColor::TableBorderLight, "Table Border Light")
	    .add(EColor::TableRowBg, "Table Row Background")
	    .add(EColor::TableRowBgAlt, "Table Row Background Alt")
	    .add(EColor::TextSelectedBg, "Text Selected Background")
	    .add(EColor::DragDropTarget, "Drag and Drop Target")
	    .add(EColor::NavHighlight, "Navigation Highlight")
	    .add(EColor::NavWindowingHighlight, "Navigation Windowing Highlight")
	    .add(EColor::NavWindowingDimBg, "Navigation Windowing Dim Background")
	    .add(EColor::ModalWindowDimBg, "Modal Window Dim Background")

	    // Dock tab options
	    .add(EColor::DockTab, "Dock Tab")
	    .add(EColor::DockTabActive, "Dock Tab Active")
	    .add(EColor::DockTabUnfocused, "Dock Tab Unfocused")
	    .add(EColor::DockTabUnfocusedActive, "Dock Tab Unfocused Active")
	    .add(EColor::DockTabHovered, "Dock Tab Hovered")

	    .add(EColor::ButtonDim, "Button Dim")
	    .add(EColor::ButtonDimHovered, "Button Dim Hovered")
	    .add(EColor::ButtonDimActive, "Button Dim Active")
	    .add(EColor::ButtonDimToggled, "Button Dim Toggled")
	    .add(EColor::ButtonDimDark, "Button Dim Dark")

	    // TODO: All ImGui sizes
	    .add(ESize::Window_Rounding, "Window Rounding")
	    .add(ESizeVec2::Window_Padding, "Window Padding")
	    .add(ESize::Frame_Rounding, "Frame Rounding")
	    .add(ESizeVec2::FramePadding, "Window Frame Padding")
	    .add(ESize::Tooltip_Rounding, "Tooltip Rounding")
	    .add(ESizeVec2::Tooltip_Padding, "Tooltip Padding")
	    .add(ESize::FloatingButtonRounding, "Floating Button Rounding");

	group("Start Window", "star")
	    .add(EColor::StartWindow_DescriptionBackground, "Description Background")
	    .add(EColor::StartWindow_WindowBackground, "Window Background")
	    .add(EColor::StartWindow_ScrollbarBackground, "Scrollbar Background")
	    .add(EColor::StartWindow_ScrollbarGrab, "Scrollbar Grab")
	    .add(EColor::StartWindow_ScrollbarGrabHovered, "Scrollbar Grab Hovered")
	    .add(EColor::StartWindow_ScrollbarGrabActive, "Scrollbar Grab Active")
	    .add(EColor::StartWindow_Separator, "Separator")
	    .add(EColor::StartWindow_DefaultButton, "Default Button")
	    .add(EColor::StartWindow_NewSceneButton, "New Scene Button")
	    .add(EColor::StartWindow_NewSceneButtonFont, "New Scene Button Font")
	    .add(EColor::StartWindow_TitleFont, "Title Font Color")
	    .add(EColor::StartWindow_DescriptionFont, "Description Font Color")
	    .add(EColor::StartWindow_YourSceneWinBackground, "Your Scene Section Background")

	    .add(ESize::StartWindow_WinWidth, "Window Width")
	    .add(ESize::StartWindow_WinHeight, "Window Height")
	    .add(ESize::StartWindow_WinRounding, "Window Rounding")
	    .addN(ESize::StartWindow_TitleVerticalOffset, "Title Vertical Offset")
	    .add(ESize::StartWindow_LeftBarWidth, "Left Bar Width")
	    .add(ESize::StartWindow_LoadButtonWidth, "Load Button Width")
	    .add(ESize::StartWindow_StartNewButtonWidth, "Start New Button Width")
	    .add(ESize::StartWindow_ButtonHeight, "Button Height")
	    .add(ESize::StartWindow_ThumbImageSize, "I3T Image Size")
	    .add(ESize::StartWindow_StartButtonWidth, "Start Button Width")
	    .add(ESize::StartWindow_FrameRounding, "Frame Rounding")
	    .add(ESize::StartWindow_ScrollbarSize, "Scrollbar Size")
	    .add(ESize::StartWindow_YourSceneWinRounding, "Your Scene Section Rounding")
	    .add(ESize::StartWindow_DotSize, "Dot Size")
	    .add(ESize::StartWindow_DotSpacing, "Dot Spacing")

	    .add(ESizeVec2::StartWindow_WinSize, "Window Size")
	    .addN(ESizeVec2::StartWindow_LogoOffset, "Logo Offset")
	    .add(ESizeVec2::StartWindow_WinPadding, "Window Padding")
	    .add(ESizeVec2::StartWindow_LeftWinPadding, "Left Window Padding")
	    .add(ESizeVec2::StartWindow_RightWinOuterPadding, "Right Window OuterPadding")
	    .add(ESizeVec2::StartWindow_RightWinInnerPadding, "Right Window InnerPadding");

	group("Tutorials", "tuts")
	    .add(EColor::TutorialBgColor, "Background")
	    .add(EColor::TutorialText, "Text")
	    .add(EColor::TutorialBarBg, "Bar Background")
	    .add(EColor::TutorialScrollbarBg, "Scrollbar Background")
	    .add(EColor::TutorialScrollbarActive, "Scrollbar Active")
	    .add(EColor::TutorialScrollbarGrab, "Scrollbar Grab")
	    .add(EColor::TutorialScrollbarHovered, "Scrollbar Hovered")
	    .add(EColor::TutorialTitleText, "Title Text")
	    .add(EColor::TutorialHighlightText, "Highlight Text")
	    .add(EColor::TutorialButtonText, "Button Text")
	    .add(EColor::TutorialButtonBg, "Button Background")
	    .add(EColor::TutorialButtonActive, "Button Active")
	    .add(EColor::TutorialButtonHovered, "Button Hovered")
	    .add(EColor::TutorialTaskBg, "Task Background")
	    .add(ESize::Tutorial_FrameRounding, "Frame Rounding")
	    .add(ESize::Tutorial_ScrollbarSize, "Scrollbar Size")
	    .add(ESize::Tutorial_ScrollbarRounding, "Scrollbar Rounding")
	    .add(ESize::Tutorial_BackButtonWidth, "Back Button Width")
	    .add(ESize::Tutorial_MainMenuButtonWidth, "Main Menu Button Width")
	    .add(ESize::Tutorial_TaskSquareXPadding, "Task Square X Padding")
	    .add(ESizeVec2::Tutorial_WindowPadding, "Window Padding");

	group("About Window", "abut")
	    .add(EColor::AboutWindow_BackgroundLeft, "AboutWindow BackgroundLeft")
	    .add(EColor::AboutWindow_BackgroundRight, "AboutWindow BackgroundRight")
	    .add(EColor::AboutWindow_Text, "AboutWindow Text");

	group("Scene View", "sgen").add(EColor::SceneViewBackground, "Scene View Background");

	group("Node Editor", "ngen")
	    .add(EColor::NodeEditorBackground, "Node Editor Background")
	    .add(EColor::NodeEditorGridColor, "Node Editor Grid Color")
	    .add(EColor::NodeEditorGridDotsColor, "Node Editor Grid Dots Color")
	    .add(EColor::NodeBg, "General Node Background")
	    .add(EColor::NodeHeader, "General Node Header")
	    .add(EColor::NodeFont, "General Node Font (text)")
	    .add(EColor::NodeBorder, "Node Border")
	    .add(EColor::Workspace_HoverBorder, "Hover Border Color")
	    .add(EColor::SelectionRectFull, "Selection rectangle full")
	    .add(EColor::SelectionRectTouch, "Selection rectangle touch")
	    .add(EColor::Workspace_SelectedBorder, "Selected Node Border")
	    .add(EColor::NodeLODButtonColorText, "LOD Button Text")
	    .add(EColor::NodeContextButtonColorText, "Context Button Text")
	    .add(EColor::NodeLODButtonColor, "LOD Button Color")
	    .add(EColor::NodeLODButtonColorActive, "LOD Button Active Color")
	    .add(EColor::NodeLODButtonColorHovered, "LOD Button Hover Color")
	    .add(ESize::Nodes_Operators_Rounding, "Nodes Operators Rounding")
	    .add(ESize::Nodes_Sequence_Rounding, "Nodes Sequence Rounding")
	    .add(ESize::Nodes_LOD_Button_Rounding, "Nodes LOD Button Rounding")
	    .add(ESize::Nodes_Border_Rounding, "Nodes Border Rounding")
	    .add(ESize::Nodes_Border_Thickness, "Nodes Border Thickness")
	    .add(ESize::Workspace_HoverBorderThickness, "Hover Border Thickness")
	    .add(ESize::Workspace_SelectedBorderThickness, "Selected Node Border Thickness")
	    .add(ESize::Nodes_Rounding, "Nodes Rounding")
	    .add(ESize::Nodes_BorderWidth, "Nodes BorderWidth")
	    .add(ESize::Nodes_LabelIndent, "Pin Label Indent")
	    .add(ESize::Nodes_HeaderLabelIndent, "Header Label Indent")
	    .add(ESize::Nodes_trackballButtonHeight, "Trackball Button Height")
	    .add(ESize::Nodes_TrackBallSensitivity, "TrackBall Sensitivity")
	    .add(ESize::Nodes_dragSpeedDefaultRatio, "Drag Speed fo Float")
	    .add(ESize::Nodes_CtrlMultiplicator, "CTRL Multiplicator")
	    .add(ESize::Nodes_SHIFTMultiplicator, "SHIFT Multiplicator")
	    .add(ESize::Nodes_ALTMultiplicator, "ALT Multiplicator")
	    .add(ESize::Nodes_InputsAlignment, "Nodes Inputs Alignment")    // Unused?
	    .add(ESize::Nodes_MiddleAlignment, "Nodes Middle Alignment")    // Unused?
	    .add(ESize::Nodes_OutputsAlignment, "Nodes Outputs Alignment")  // Unused?
	    .add(ESize::Nodes_leftSideSpacing, "Nodes Left Side spacing")   // Unused?
	    .add(ESize::Nodes_rightSideSpacing, "Nodes Right Side spacing") // Unused?
	    .add(ESize::Nodes_Transformation_TrackingMarkSize, "Nodes Transformation TrackingMarkSize")
	    .add(ESize::Default_VisiblePrecision, "Nodes Default Visible Precision")
	    .add(ESize::Default_VisibleQuaternionPrecision, "Nodes Default Visible Precision For Quaternions")
	    .add(ESize::Default_InactiveMark, "Nodes Default Inactive Part Marker")
	    .addN(ESizeVec2::Nodes_NewPositionShift, "New Node Position Shift")
	    .add1(ESizeVec2::Nodes_PinSize, "Nodes Pin Size")
	    .add1(ESizeVec2::Nodes_PinSize_MatrixMul, "Nodes Pin Size Matrix Mul")
	    .add(ESizeVec2::Nodes_PivotAlignment, "Nodes Pivot Alignment")
	    .add(ESizeVec2::Nodes_PinSpacing, "Nodes Pin Spacing")
	    .add(ESizeVec2::Nodes_ItemsSpacing, "Nodes Items Spacing")
	    .add(ESizeVec2::Nodes_PivotSize, "Nodes Pivot Size")
	    .add(ESizeVec2::Nodes_InputsSize, "Nodes Inputs Size")
	    .add(ESizeVec2::Nodes_MiddleSize, "Nodes Middle Size")
	    .add(ESizeVec2::Nodes_OutputSize, "Nodes Output Size")
	    .add(ESizeVec2::Nodes_LODButtonSize, "Nodes LOD Button Size")
	    .add(ESizeVec2::Builder_ItemSpacing, "Builder Item Spacing");

	group("Node Editor Links", "nlnk", 1)
	    .add(ESize::Links_ControlpointsPositionFraction, "Link Control Point Distance Scaling Factor",
	         "Multiplier of distance between link control point and its pin, a fraction of distance of the link's "
	         "start and end positions.")
	    .add(ESize::Links_ControlpointsPositionMin, "Link Min Control Point X Distance")
	    .add(ESize::Links_ControlpointsPositionMax, "Link Max Control Point X Distance")
	    .add(ESize::Links_Thickness, "Link Thickness")
	    .add(ESize::Links_ThicknessSelected, "Link Thickness when selected")
	    .addF01(ESize::Links_selected_alpha, "Link Selected Alpha")
	    .addF01(EColor::Links_selected_colorShift, "selected_colorShift");

	group("Node Editor Pins", "npin", 1)
	    .add(ESize::Pins_IconPadding, "Pin Icon Padding")
	    .add(EColor::DisabledPinColor, "Disabled Pin Color")
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

	group("Node Editor Operators", "nops", 1)
	    .add(EColor::NodeBgOperator, "Operator Background")
	    .add(EColor::NodeHeaderOperator, "Operator Header");

	group("Node Editor Transformations", "ntrs", 1)
	    .add(ESizeVec2::Nodes_Sequence_DummySpaceSize, "Sequence Dummy Space Size")
	    .add(EColor::NodeBgTransformation, "Transform Background")
	    .add(EColor::NodeHeaderTranformation, "Transform Header")
	    .add(EColor::Nodes_Transformation_ValidIcon_bgShape, "Nodes Transformation ValidIcon bgShape")
	    .add(EColor::Nodes_Transformation_ValidIcon_bgInner, "Nodes Transformation ValidIcon bgInner")
	    .add(EColor::Nodes_Transformation_ValidIcon_fgShape, "Nodes Transformation ValidIcon fgShape")
	    .add(EColor::Nodes_Transformation_ValidIcon_fgInner, "Nodes Transformation ValidIcon fgInner")
	    .add(EColor::Nodes_Transformation_ValidIcon_padding, "Nodes Transformation ValidIcon padding",
	         "Padding stored as color")
	    .add(EColor::Nodes_Tracking_ColorActive, "Nodes Tracking Color Active")
	    .add(EColor::Nodes_Tracking_ColorInactive, "Nodes Tracking Color Inactive")
	    .add(EColor::Nodes_Tracking_Cursor, "Nodes Tracking Cursor")
	    .add(EColor::Nodes_Tracking_OverlayActive, "Nodes Tracking Overlay Active")
	    .add(EColor::Nodes_Tracking_OverlayInactive, "Nodes Tracking Overlay Inactive");

	group("Node Editor Popups", "npop", 1)
	    .add(EColor::Nodes_ConnectionPossible, "Connection is possible (text)")
	    .add(EColor::Nodes_ConnectionNotPossible, "Connection is not possible (text)")
	    .add(EColor::Nodes_CreateNode, "Create node popup");

	group("Node Editor Floats", "nflo", 1)
	    .add(EColor::FloatBg, "Float Background")
	    .add(EColor::FloatBgHovered, "Float Background Hovered")
	    .add(EColor::FloatBgActive, "Float Background Active")
	    .add(EColor::Synergies_FloatBg, "Synergies FloatBg")
	    .add(EColor::Synergies_FloatBgHovered, "Synergies FloatBgHovered")
	    .add(EColor::Synergies_FloatBgActive, "Synergies FloatBgActive")
	    .addF01(ESize::Float_inactive_alphaMultiplicator, "Float Inactive Alpha Factor")
	    .addF(ESize::Nodes_FloatCharacterWidthMultiplier, "Float Character Width Multiplier",
	          "Factor of font size determining the width of a single character.")
	    .addF1(ESize::Nodes_FloatMinCharacters, "Float Minimum Character Count")
	    .add(ESize::Nodes_FloatInnerPadding, "Float Inner Padding",
	         "Horizontal padding inside float data items (floats, vectors, matrices etc.).")
	    .add(ESizeVec2::Nodes_FloatPadding, "Nodes Float Padding")
	    .add(ESizeVec2::Nodes_FloatCycleButtonSize, "Nodes Float Cycle Button Size");


	group("Node Editor ScreenNode", "nscr", 1)
	    .add(EColor::Nodes_Screen_resizeBtn_bgShape, "Nodes Screen resizeBtn bgShape")
	    .add(EColor::Nodes_Screen_resizeBtn_bgInner, "Nodes Screen resizeBtn bgInner")
	    .add(EColor::Nodes_Screen_resizeBtn_fgShape, "Nodes Screen resizeBtn fgShape")
	    .add(EColor::Nodes_Screen_resizeBtn_fgInner, "Nodes Screen resizeBtn fgInner")
	    .add(EColor::Nodes_Screen_noInput_background, "Nodes_Screen_noInput_background")
	    .add(EColor::Nodes_Screen_noInput_text, "Nodes_Screen_noInput_text")
	    .add(ESizeVec2::Nodes_Screen_resizeButtonSize, "Screen Resize Button Size")
	    .add(ESizeVec2::Nodes_ScreenTextureSize, "Nodes Screen Texture Size");

	group("Node Editor Cycle", "ncyc", 1)
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

	// Populate g_XXXNames containers from theme variable groups, also check if all style enum fields are covered.
	initializeAndValidateThemeVariables();
}

void Theme::initImGuiStyle()
{
	ImGuiIO& io = ImGui::GetIO();

	// Reset ImGuiStyle
	ImGui::GetCurrentContext()->Style = ImGuiStyle();

	// Default styling
	ImGui::StyleColorsDark();

	// Extra theme agnostic tweaks

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform
	// windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f; // disable alpha
	}
}

void Theme::updateDiwneStyleFromTheme() const
{
	float dpiScale = getDpiScale();

	DIWNE::NodeEditor& editor = I3T::getWorkspace().getNodeEditor();
	editor.setDpiScale(dpiScale); // Inform the NodeEditor that DPI scaling is at play, affects screen <-> diwne coords

	// I3T Theme variable POINTERS are passed to DIWNE style. There pointers are NOT dpi scaled.
	// Note that get() calls ARE dpi scaled, getPtr() calls aren't (since they point to underlying values)
	// Pointers are passed so that Style can be dynamically updated without explicitly updating all values

	using DIWNE::Style;
	DIWNE::StyleBase& style = editor.styleBase();

	style.set(Style::GRID_COLOR, I3T::getUI()->getTheme().getPtr(EColor::NodeEditorGridColor));
	style.set(Style::GRID_DOTS_COLOR, I3T::getUI()->getTheme().getPtr(EColor::NodeEditorGridDotsColor));

	style.set(Style::NODE_BG, I3T::getUI()->getTheme().getPtr(EColor::NodeBg));
	style.set(Style::NODE_HEADER_BG, I3T::getUI()->getTheme().getPtr(EColor::NodeHeader));

	style.set(Style::NODE_ROUNDING, I3T::getUI()->getTheme().getPtr(ESize::Nodes_Rounding), true);

	style.set(Style::SELECTION_ROUNDING, I3T::getUI()->getTheme().getPtr(ESize::Nodes_Border_Rounding), true);
	style.set(Style::SELECTED_BORDER_WIDTH, I3T::getUI()->getTheme().getPtr(ESize::Workspace_SelectedBorderThickness),
	          true);
	style.set(Style::SELECTED_BORDER_COLOR, I3T::getUI()->getTheme().getPtr(EColor::Workspace_SelectedBorder));
	style.set(Style::HOVER_BORDER_WIDTH, I3T::getUI()->getTheme().getPtr(ESize::Workspace_HoverBorderThickness), true);
	style.set(Style::HOVER_BORDER_COLOR, I3T::getUI()->getTheme().getPtr(EColor::Workspace_HoverBorder));

	style.set(Style::LINK_WIDTH, I3T::getUI()->getTheme().getPtr(ESize::Links_Thickness), true);
	style.set(Style::LINK_SELECTED_WIDTH, I3T::getUI()->getTheme().getPtr(ESize::Links_ThicknessSelected), true);

	style.set(Style::SELECTION_RECT_FULL_COLOR, I3T::getUI()->getTheme().getPtr(EColor::SelectionRectFull));
	style.set(Style::SELECTION_RECT_TOUCH_COLOR, I3T::getUI()->getTheme().getPtr(EColor::SelectionRectTouch));

	// Update StyleOverrides

	// CoreNode
	m_nodeStyle.setEditor(&editor);
	m_nodeStyle.setOverride<ImVec4*>(DIWNE::Style::NODE_BG, I3T::getTheme().getPtr(EColor::NodeBg));
	m_nodeStyle.setOverride<ImVec4*>(DIWNE::Style::NODE_HEADER_BG, I3T::getTheme().getPtr(EColor::NodeHeader));

	// Transformation
	m_transformationStyle.setEditor(&editor);
	m_transformationStyle.setOverride<ImVec4*>(DIWNE::Style::NODE_BG,
	                                           I3T::getTheme().getPtr(EColor::NodeBgTransformation));
	m_transformationStyle.setOverride<ImVec4*>(DIWNE::Style::NODE_HEADER_BG,
	                                           I3T::getTheme().getPtr(EColor::NodeHeaderTranformation));

	// Operator
	m_operatorStyle.setEditor(&editor);
	m_operatorStyle.setOverride<ImVec4*>(DIWNE::Style::NODE_BG, I3T::getTheme().getPtr(EColor::NodeBgOperator));
	m_operatorStyle.setOverride<ImVec4*>(DIWNE::Style::NODE_HEADER_BG,
	                                     I3T::getTheme().getPtr(EColor::NodeHeaderOperator));
}

void Theme::apply()
{
	ImGuiStyle& style = ImGui::GetStyle();

	// ImGuiStyle colors
	style.Colors[ImGuiCol_Text] = m_colors[EColor::Text];
	style.Colors[ImGuiCol_TextDisabled] = m_colors[EColor::TextDisabled];
	style.Colors[ImGuiCol_WindowBg] = m_colors[EColor::WindowBackground];
	style.Colors[ImGuiCol_ChildBg] = m_colors[EColor::ChildBackground];
	style.Colors[ImGuiCol_PopupBg] = m_colors[EColor::PopupBackground];
	style.Colors[ImGuiCol_Border] = m_colors[EColor::Border];
	style.Colors[ImGuiCol_FrameBg] = m_colors[EColor::FrameBg];
	style.Colors[ImGuiCol_FrameBgHovered] = m_colors[EColor::FrameBgHovered];
	style.Colors[ImGuiCol_FrameBgActive] = m_colors[EColor::FrameBgActive];
	style.Colors[ImGuiCol_TitleBg] = m_colors[EColor::WindowTitleBg];
	style.Colors[ImGuiCol_TitleBgActive] = m_colors[EColor::WindowTitleBgActive];
	style.Colors[ImGuiCol_TitleBgCollapsed] = m_colors[EColor::WindowTitleBgCollapsed];
	style.Colors[ImGuiCol_MenuBarBg] = m_colors[EColor::MenuBarBackground];
	style.Colors[ImGuiCol_ScrollbarBg] = m_colors[EColor::ScrollbarBg];
	style.Colors[ImGuiCol_ScrollbarGrab] = m_colors[EColor::ScrollbarGrab];
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = m_colors[EColor::ScrollbarGrabHovered];
	style.Colors[ImGuiCol_ScrollbarGrabActive] = m_colors[EColor::ScrollbarGrabActive];
	style.Colors[ImGuiCol_CheckMark] = m_colors[EColor::CheckMark];
	style.Colors[ImGuiCol_SliderGrab] = m_colors[EColor::SliderGrab];
	style.Colors[ImGuiCol_SliderGrabActive] = m_colors[EColor::SliderGrabActive];
	style.Colors[ImGuiCol_Button] = m_colors[EColor::Button];
	style.Colors[ImGuiCol_ButtonHovered] = m_colors[EColor::ButtonHovered];
	style.Colors[ImGuiCol_ButtonActive] = m_colors[EColor::ButtonActive];
	style.Colors[ImGuiCol_Header] = m_colors[EColor::Header];
	style.Colors[ImGuiCol_HeaderHovered] = m_colors[EColor::HeaderHovered];
	style.Colors[ImGuiCol_HeaderActive] = m_colors[EColor::HeaderActive];
	style.Colors[ImGuiCol_Separator] = m_colors[EColor::Separator];
	style.Colors[ImGuiCol_SeparatorHovered] = m_colors[EColor::SeparatorHovered];
	style.Colors[ImGuiCol_SeparatorActive] = m_colors[EColor::SeparatorActive];
	style.Colors[ImGuiCol_ResizeGrip] = m_colors[EColor::ResizeGrip];
	style.Colors[ImGuiCol_ResizeGripHovered] = m_colors[EColor::ResizeGripHovered];
	style.Colors[ImGuiCol_ResizeGripActive] = m_colors[EColor::ResizeGripActive];
	style.Colors[ImGuiCol_Tab] = m_colors[EColor::Tab];
	style.Colors[ImGuiCol_TabHovered] = m_colors[EColor::TabHovered];
	style.Colors[ImGuiCol_TabActive] = m_colors[EColor::TabActive];
	style.Colors[ImGuiCol_TabUnfocused] = m_colors[EColor::TabUnfocused];
	style.Colors[ImGuiCol_TabUnfocusedActive] = m_colors[EColor::TabUnfocusedActive];
	style.Colors[ImGuiCol_DockingPreview] = m_colors[EColor::DockingPreview];
	style.Colors[ImGuiCol_DockingEmptyBg] = m_colors[EColor::DockingEmptyBg];
	style.Colors[ImGuiCol_PlotLines] = m_colors[EColor::PlotLines];
	style.Colors[ImGuiCol_PlotLinesHovered] = m_colors[EColor::PlotLinesHovered];
	style.Colors[ImGuiCol_PlotHistogram] = m_colors[EColor::PlotHistogram];
	style.Colors[ImGuiCol_PlotHistogramHovered] = m_colors[EColor::PlotHistogramHovered];
	style.Colors[ImGuiCol_TableHeaderBg] = m_colors[EColor::TableHeaderBg];
	style.Colors[ImGuiCol_TableBorderStrong] = m_colors[EColor::TableBorderStrong];
	style.Colors[ImGuiCol_TableBorderLight] = m_colors[EColor::TableBorderLight];
	style.Colors[ImGuiCol_TableRowBg] = m_colors[EColor::TableRowBg];
	style.Colors[ImGuiCol_TableRowBgAlt] = m_colors[EColor::TableRowBgAlt];
	style.Colors[ImGuiCol_TextSelectedBg] = m_colors[EColor::TextSelectedBg];
	style.Colors[ImGuiCol_DragDropTarget] = m_colors[EColor::DragDropTarget];
	style.Colors[ImGuiCol_NavHighlight] = m_colors[EColor::NavHighlight];
	style.Colors[ImGuiCol_NavWindowingHighlight] = m_colors[EColor::NavWindowingHighlight];
	style.Colors[ImGuiCol_NavWindowingDimBg] = m_colors[EColor::NavWindowingDimBg];
	style.Colors[ImGuiCol_ModalWindowDimBg] = m_colors[EColor::ModalWindowDimBg];

	// TODO: Implement and add ImGui sizes

	style.WindowRounding = get(ESize::Window_Rounding);
	style.WindowPadding = get(ESizeVec2::Window_Padding);
	style.FrameRounding = get(ESize::Frame_Rounding);
	style.FramePadding = get(ESizeVec2::FramePadding);

	style.TabRounding = 2.0f * m_dpiScale;

	// Show borders, these sizes are not scaled by ImGuiStyle::ScaleAllSizes() so we handle it here manually
	style.ChildBorderSize = IM_TRUNC(std::max(1.0f, m_dpiScale));
	style.PopupBorderSize = IM_TRUNC(std::max(1.0f, m_dpiScale));
	style.WindowBorderSize = IM_TRUNC(std::max(1.0f, m_dpiScale));

	updateDiwneStyleFromTheme();
}

ImFont* Theme::get(EFont font)
{
	return I3T::getUI()->getFontManager().getFont(m_fontsAssoc[font]);
}

std::map<EColor, std::string>& Theme::getColorNames()
{
	return g_ColorNames;
}

std::map<ESize, std::string>& Theme::getSizeNames()
{
	return g_SizeNames;
}

std::map<ESizeVec2, std::string>& Theme::getSizeVecNames()
{
	return g_SizeVecNames;
}

ThemeGroup& Theme::group(const char* name, const char* id, int indent)
{
	s_variables.emplace_back(name, id, indent);

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
	                        L"AppsUseLightTheme", RRF_RT_REG_DWORD,
	                        // expected value type
	                        nullptr, buffer.data(), &cbData);

	if (res != ERROR_SUCCESS)
	{
		LOG_ERROR("[THEME] Failed to determine OS theme mode! error_code: " + std::to_string(res));
		return false;
	}

	// convert bytes written to our buffer to an int, assuming little-endian
	auto i = int(buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0]);

	return i == 1;
#endif

	return false;
}
