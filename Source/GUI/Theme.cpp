#include "Theme.h"

#include "imgui_node_editor.h"

#include "Config.h"
#include "Core/API.h"

static std::map<EColor, const char*> g_ColorNames;
static std::map<ESize, const char*> g_SizeNames;
static std::map<ESizeVec2, const char*> g_SizeVecNames;

Theme Theme::createDefault()
{
	Theme theme;

	theme.initFonts();

	theme.m_name = "default";

	return theme;
}

Theme::Theme()
{
	initDefaultProperties();
}

Theme::Theme(std::string name, const Theme::Colors& colors, const Theme::Sizes& sizes,
						 const Theme::SizesVec& sizesVec)
{
	initDefaultProperties();

	m_name = std::move(name);
	copyProperties(m_colors, colors);
	copyProperties(m_sizes, sizes);
	copyProperties(m_sizesVec2, sizesVec);
}

void Theme::initFonts()
{
	auto& io = ImGui::GetIO();

	m_fontsAssoc.insert(std::pair(EFont::MenuLarge, 0));
	m_fontsAssoc.insert(std::pair(EFont::TutorialText, 0));
	m_fontsAssoc.insert(std::pair(EFont::Button, 1));
	m_fontsAssoc.insert(std::pair(EFont::Tab, 1));
	m_fontsAssoc.insert(std::pair(EFont::Node, 2));
	m_fontsAssoc.insert(std::pair(EFont::MenuSmall, 2));
	m_fontsAssoc.insert(std::pair(EFont::Title, 3));
	m_fontsAssoc.insert(std::pair(EFont::TitleSmall, 4));

	io.FontDefault = get(EFont::MenuLarge);
}

void Theme::initDefaultProperties()
{
	set(EColor::Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	set(EColor::WindowBackground, ImVec4(0.439f, 0.439f, 0.455f, 1.00f));
	set(EColor::PopupBackground, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	set(EColor::MenuBarBackground, ImVec4(0.40f, 0.38f, 0.369f, 1.00f));
	set(EColor::Border, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	set(EColor::PrimaryColor, ImVec4(0.278f, 0.278f, 0.286f, 1.00f));
	set(EColor::ActiveColor, ImVec4(0.259f, 0.588f, 0.980f, 1.00f));
  set(EColor::FloatBg, ImVec4(0.16f, 0.29f, 0.48f, 0.54f));
	set(EColor::FloatBgHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.4f));
	set(EColor::FloatBgActive, ImVec4(0.26f, 0.59f, 0.98f, 0.67f));

	set(EColor::TutorialBgColor, createColor(232, 232, 232, 255));

	// Node Editor
	set(EColor::Nodes_FloatText, ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.00f));
	set(EColor::NodeEditorBg, ImVec4(112.0f / 255.0f, 112.0f / 255.0f, 116.0f / 255.0f, 1.00f));
	set(EColor::Nodes_ConnectionPossible, createColor(0,255,0, 255));
	set(EColor::Nodes_ConnectionNotPossible, createColor(255,0,0, 255));
	set(EColor::Nodes_CreateNode, createColor(32, 45, 32, 180));

	set(EColor::Builder_NodePadding, ImVec4(0, 0, 0, 0));

	//pin colors
	set(EColor::PulsePin, createColor(164, 58, 190, 255));
	set(EColor::FloatPin, createColor(58, 144, 66, 255));
	set(EColor::MatrixPin, createColor(178, 71, 66, 255));
	set(EColor::QuatPin, createColor(178, 144, 66, 255));
	set(EColor::Vec3Pin, createColor(58, 84, 187, 255));
	set(EColor::Vec4Pin, createColor(106, 96, 67, 255));
	set(EColor::ScreenPin, createColor(51, 150, 215, 255));
	set(EColor::MatrixMulPin, createColor(68, 201, 156, 255));

	//pin inner colors
	set(EColor::InnerPulsePin, createColor(255, 255, 255, 255));
	set(EColor::InnerFloatPin, createColor(255, 255, 255, 255));
	set(EColor::InnerMatrixPin, createColor(255, 255, 255, 255));
	set(EColor::InnerQuatPin, createColor(255, 255, 255, 255));
	set(EColor::InnerVec3Pin, createColor(255, 255, 255, 255));
	set(EColor::InnerVec4Pin, createColor(255, 255, 255, 255));
	set(EColor::InnerScreenPin, createColor(255, 255, 255, 255));
	set(EColor::InnerMatrixMulPin, createColor(255, 255, 255, 255));


	// Folta operator color set
	set(EColor::NodeBgOperator, ImVec4(83.0f / 255.0f, 101.0f / 255.0f, 146.0f / 255.0f, 1.00f));
	set(EColor::NodeHeaderOperator, ImVec4(73.0f / 255.0f, 91.0f / 255.0f, 136.0f / 255.0f, 1.00f));
	set(EColor::FloatBgOperator, ImVec4(77.0f / 255.0f, 85.0f / 255.0f, 106.0f / 255.0f, 1.00f));
	set(EColor::FloatBgOperatorActive, ImVec4(97.0f / 255.0f, 105.0f / 255.0f, 126.0f / 255.0f, 1.00f));
	set(EColor::FloatBgOperatorHovered, ImVec4(87.0f / 255.0f, 95.0f / 255.0f, 116.0f / 255.0f, 1.00f));

	// Folta transformation color set
	set(EColor::NodeBgTransformation, ImVec4(137.0f / 255.0f, 115.0f / 255.0f, 59.0f / 255.0f, 1.00f));
	set(EColor::NodeHeaderTranformation, ImVec4(127.0f / 255.0f, 105.0f / 255.0f, 49.0f / 255.0f, 1.00f));
	set(EColor::FloatBgTransformation, ImVec4(91.0f / 255.0f, 84.0f / 255.0f, 67.0f / 255.0f, 1.00f));
	set(EColor::FloatBgTransformationActive, ImVec4(111.0f / 255.0f, 104.0f / 255.0f, 87.0f / 255.0f, 1.00f));
	set(EColor::FloatBgTransformationHovered, createColor(101, 94, 77, 255));


	m_fontsAssoc.insert(std::pair(EFont::MenuLarge, 0));
	m_fontsAssoc.insert(std::pair(EFont::TutorialText, 0));
	m_fontsAssoc.insert(std::pair(EFont::Button, 1));
	m_fontsAssoc.insert(std::pair(EFont::Tab, 1));
	m_fontsAssoc.insert(std::pair(EFont::Node, 2));
	m_fontsAssoc.insert(std::pair(EFont::MenuSmall, 2));
	m_fontsAssoc.insert(std::pair(EFont::Title, 3));
	m_fontsAssoc.insert(std::pair(EFont::TaskTitle, 4));

	// Tutorial fonts.
	m_fontsAssoc.insert(std::pair(EFont::Header, 5));
	m_fontsAssoc.insert(std::pair(EFont::I3TTitle, 7));
	m_fontsAssoc.insert(std::pair(EFont::I3TDescription, 8));
	m_fontsAssoc.insert(std::pair(EFont::IntroItemTitle, 6));
	m_fontsAssoc.insert(std::pair(EFont::IntroItemDescription, 2));


	m_sizes[ESize::Nodes_FloatMargin] = 1.0f;
	m_sizes[ESize::Nodes_FloatWidth] = 25.0f;
	m_sizes[ESize::Nodes_Rounding] = 0.0f;
	m_sizes[ESize::Nodes_BorderWidth] = 0.0f;
	m_sizes[ESize::Nodes_LabelIndent] = 3.0f;
	m_sizes[ESize::Nodes_HeaderLabelIndent] = 2.0f;
	m_sizes[ESize::Nodes_LinkThickness] = 3.0f;
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

	m_sizesVec2[ESizeVec2::Window_FramePadding] = ImVec2(0.0f, 4.0f);

	m_sizesVec2[ESizeVec2::Nodes_ItemsSpacing] = ImVec2(2.0f, 3.0f);
	m_sizesVec2[ESizeVec2::Nodes_FloatPadding] = ImVec2(0.0f, 1.0f);
	m_sizesVec2[ESizeVec2::Nodes_PinSpacing] = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_PivotAlignment] = ImVec2(0.0f, 0.5f);
	m_sizesVec2[ESizeVec2::Nodes_PivotSize] = ImVec2(0.0f, 0.0f);

	m_sizesVec2[ESizeVec2::Nodes_InputsSize] = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_MiddleSize] = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_OutputSize] = ImVec2(0.0f, 0.0f);

	m_sizesVec2[ESizeVec2::Nodes_IconSize] = ImVec2(12.0f, 12.0f);
	m_sizesVec2[ESizeVec2::Nodes_FloatCycleButtonSize] = ImVec2(32.0f, 32.0f);
	m_sizesVec2[ESizeVec2::Nodes_ScreenTextureSize] = ImVec2(130.0f, 130.0f);

	m_sizesVec2[ESizeVec2::Builder_ItemSpacing] = ImVec2(0.0f, 0.0f);
}

void Theme::initNames()
{
	g_ColorNames[EColor::Text] = "Text";
	g_ColorNames[EColor::Border] = "Border";
	g_ColorNames[EColor::WindowBackground] = "Window Background";
	g_ColorNames[EColor::PopupBackground] = "Popup Background";
	g_ColorNames[EColor::MenuBarBackground] = "Menu Bar Background";
	g_ColorNames[EColor::PrimaryColor] = "Primary Color (tabs, tiles, ...)";
	g_ColorNames[EColor::ActiveColor] = "Active Color";
	g_ColorNames[EColor::TabColor] = "Tab Color";

	g_ColorNames[EColor::NodeBgOperator] = "Operator Background";
	g_ColorNames[EColor::NodeHeaderOperator] = "Operator Header";
	g_ColorNames[EColor::FloatBgOperator] = "Operator Float Background";
	g_ColorNames[EColor::FloatBgOperatorActive] = "Operator Active Float Background";
	g_ColorNames[EColor::FloatBgOperatorHovered] = "Operator Hovered Float Background";

	g_ColorNames[EColor::NodeBgTransformation] = "Transform Background";
	g_ColorNames[EColor::NodeHeaderTranformation] = "Transform Header";
	g_ColorNames[EColor::FloatBgTransformation] = "Transform Float Background";
	g_ColorNames[EColor::FloatBgTransformationActive] = "Transform Float Active";
	g_ColorNames[EColor::FloatBgTransformationHovered] = "Transform Float Hovered";

	g_ColorNames[EColor::NodeEditorBg] = "Node Editor Background";

	g_ColorNames[EColor::PulsePin] = "Pulse Pin";
	g_ColorNames[EColor::FloatPin] = "Float Pin";
	g_ColorNames[EColor::MatrixPin] = "Matrix Pin";
	g_ColorNames[EColor::QuatPin] = "Quaternion Pin";
	g_ColorNames[EColor::Vec3Pin] = "Vector 3 Pin";
	g_ColorNames[EColor::Vec4Pin] = "Vector 4 Pin";
	g_ColorNames[EColor::ScreenPin] = "Screen Pin";

	g_ColorNames[EColor::InnerPulsePin] = "Inner Color Pulse";
	g_ColorNames[EColor::InnerFloatPin] = "Inner Color Float";
	g_ColorNames[EColor::InnerVec3Pin] = "Inner Color Vec3";
	g_ColorNames[EColor::InnerVec4Pin] = "Inner Color Vec4";
	g_ColorNames[EColor::InnerMatrixPin] = "Inner Color Matrix";
	g_ColorNames[EColor::InnerQuatPin] = "Inner Color Quat";
	g_ColorNames[EColor::InnerMatrixMulPin] = "Inner Color MatrixMul";
	g_ColorNames[EColor::InnerScreenPin] = "Inner Color Screen";

	g_ColorNames[EColor::Nodes_FloatText] = "Text in cells";
	g_ColorNames[EColor::Nodes_ConnectionPossible] = "Connection is possible";
	g_ColorNames[EColor::Nodes_ConnectionNotPossible] = "Connection is not possible";
	g_ColorNames[EColor::Nodes_CreateNode] = "Create node popup";

	g_ColorNames[EColor::Builder_NodePadding] = "Builder Padding (Not a color!)";


	g_SizeNames[ESize::Nodes_Rounding] = "Nodes Rounding";
	g_SizeNames[ESize::Nodes_FloatWidth] = "Nodes Float Width";
	g_SizeNames[ESize::Nodes_FloatMargin] = "Nodes Float Margin";
	g_SizeNames[ESize::Nodes_BorderWidth] = "Nodes BorderWidth";

	g_SizeNames[ESize::Nodes_LabelIndent] = "pin labels Indent";
	g_SizeNames[ESize::Nodes_HeaderLabelIndent] = "Header Label Indent";
	g_SizeNames[ESize::Nodes_LinkThickness] = "Link Thickness";
	g_SizeNames[ESize::Nodes_trackballButtonHeight] = "Trackball button height";
	g_SizeNames[ESize::Nodes_TrackBallSensitivity] = "Trackball Sensitivity";

	g_SizeNames[ESize::Nodes_FloatInnerPadding] = "Float inner padding";

	g_SizeNames[ESize::Nodes_dragSpeedDefaulrRatio] = "Drag speed fo float";
	g_SizeNames[ESize::Nodes_CtrlMultiplicator] = "CTRL Multiplicator";
	g_SizeNames[ESize::Nodes_SHIFTMultiplicator] = "SHIFT Multiplicator";
	g_SizeNames[ESize::Nodes_ALTMultiplicator] = "ALT Multiplicator";

	g_SizeNames[ESize::Nodes_InputsAlignment] = "Nodes Inputs Alignment";
	g_SizeNames[ESize::Nodes_MiddleAlignment] = "Nodes Middle Alignment";
	g_SizeNames[ESize::Nodes_OutputsAlignment] = "Nodes Outputs Alignment";

	g_SizeNames[ESize::Nodes_leftSideSpacing] = "Nodes left side spacing";
	g_SizeNames[ESize::Nodes_rightSideSpacing] = "Nodes right side spacing";


	g_SizeVecNames[ESizeVec2::Window_FramePadding] = "Windows Frame Padding";
	g_SizeVecNames[ESizeVec2::Nodes_IconSize] = "Nodes Icon Size";
	g_SizeVecNames[ESizeVec2::Nodes_PivotAlignment] = "Nodes Pivot Alignment";
	g_SizeVecNames[ESizeVec2::Nodes_PinSpacing] = "Nodes Pin Spacing";
	g_SizeVecNames[ESizeVec2::Nodes_ItemsSpacing] = "Nodes Items Spacing";
	g_SizeVecNames[ESizeVec2::Nodes_FloatPadding] = "Nodes Float Padding";

	g_SizeVecNames[ESizeVec2::Nodes_PivotSize] = "Nodes Pivot size";

	g_SizeVecNames[ESizeVec2::Nodes_InputsSize] = "Nodes Inputs Size";
	g_SizeVecNames[ESizeVec2::Nodes_MiddleSize] = "Nodes Middle Size";
	g_SizeVecNames[ESizeVec2::Nodes_OutputSize] = "Nodes Output Size";

	g_SizeVecNames[ESizeVec2::Nodes_FloatCycleButtonSize] = "Nodes Float Cycle button size";
	g_SizeVecNames[ESizeVec2::Nodes_ScreenTextureSize] = "Nodes Screen Texture Size";

	g_SizeVecNames[ESizeVec2::Builder_ItemSpacing] = "Builder Item Spacing";
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
	style.ChildBorderSize = 0.0f;
	style.PopupBorderSize = 0.0f;
	style.WindowBorderSize = 0.0f;

	// SS, MH please check this
	//---------------------------------------------------------------------------------------
	auto& nodesStyle = ax::NodeEditor::GetStyle();
	nodesStyle.NodeRounding = m_sizes[ESize::Nodes_Rounding];
	nodesStyle.NodeBorderWidth = m_sizes[ESize::Nodes_BorderWidth];
	nodesStyle.Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg] = m_colors[EColor::NodeBgOperator];
	nodesStyle.Colors[ax::NodeEditor::StyleColor::StyleColor_Bg] = m_colors[EColor::NodeEditorBg];

	//---------------------------------------------------------------------------------------
}

ImFont* Theme::get(EFont font)
{
	return I3T::getUI()->getFonts()[m_fontsAssoc[font]];
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

void Theme::operatorColorTheme()
{
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg] = m_colors[EColor::NodeBgOperator];
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_FrameBg] = m_colors[EColor::FloatBgOperator];
	style.Colors[ImGuiCol_FrameBgHovered] = m_colors[EColor::FloatBgOperatorHovered];
	style.Colors[ImGuiCol_FrameBgActive] = m_colors[EColor::FloatBgOperatorActive];
}

void Theme::transformationColorTheme()
{
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg] =
			m_colors[EColor::NodeBgTransformation];
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_FrameBg] = m_colors[EColor::FloatBgTransformation];
	style.Colors[ImGuiCol_FrameBgHovered] = m_colors[EColor::FloatBgTransformationHovered];
	style.Colors[ImGuiCol_FrameBgActive] = m_colors[EColor::FloatBgTransformationActive];
}

bool isVectorsEqual(ImVec4 a, ImVec4 b)
{
	/// \todo Comparing float values!
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w ? true : false;
}

ImVec4 Theme::getHeader()
{
	if (isVectorsEqual(ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg],
										 m_colors[EColor::NodeBgOperator]))
	{
		return m_colors[EColor::NodeHeaderOperator];
	}
	if (isVectorsEqual(ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg],
										 m_colors[EColor::NodeBgTransformation]))
	{
		return m_colors[EColor::NodeHeaderTranformation];
	}
	return ImVec4(0, 0, 0, 0);
}

void Theme::returnFloatColorToDefault(){
  ImGuiStyle& style = ImGui::GetStyle();
  style.Colors[ImGuiCol_FrameBg] = m_colors[EColor::FloatBg];
	style.Colors[ImGuiCol_FrameBgHovered] = m_colors[EColor::FloatBgHovered];
	style.Colors[ImGuiCol_FrameBgActive] = m_colors[EColor::FloatBgHovered];
}
