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


	// Node Editor
	set(EColor::NodeEditorBg, ImVec4(112.0f / 255.0f, 112.0f / 255.0f, 116.0f / 255.0f, 1.00f));
	set(EColor::PulseLink, ImVec4(1.009f, 1.00f, 1.00f, 1.00f));
	set(EColor::FloatLink, ImVec4(1.009f, 1.00f, 1.00f, 1.00f));
	set(EColor::MatrixLink, ImVec4(68.0f / 255.0f, 201.0f / 255.0f, 156.0f / 255.0f, 1.00f));
	set(EColor::QuatLink, ImVec4(124.0f / 255.0f, 21.0f / 255.0f, 153.0f / 255.0f, 1.00f));
	set(EColor::Vec3Link, ImVec4(218.0f / 255.0f, 0.0f / 255.0f, 183.0f / 255.0f, 1.00f));
	set(EColor::Vec4Link, ImVec4(218.0f / 255.0f, 0.0f / 255.0f, 183.0f / 255.0f, 1.00f));
	set(EColor::ScreenLink, ImVec4(218.0f / 255.0f, 0.0f / 255.0f, 183.0f / 255.0f, 1.00f));

	set(EColor::TutorialBgColor, createColor(232, 232, 232, 255));

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


	m_sizesVec2[ESizeVec2::Window_FramePadding] = ImVec2(8.0f, 4.0f);

	m_sizesVec2[ESizeVec2::Nodes_ItemsSpacing] = ImVec2(2.0f, 3.0f);
	m_sizesVec2[ESizeVec2::Nodes_FloatPadding] = ImVec2(8.0f, 1.0f);
	m_sizesVec2[ESizeVec2::Nodes_PinSpacing] = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_PivotAlignment] = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_IconSize] = ImVec2(12.0f, 12.0f);
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

	g_ColorNames[EColor::NodeHeader] = "Node Header";
	g_ColorNames[EColor::NodeEditorBg] = "Node Editor Background";

	g_ColorNames[EColor::PulseLink] = "Pulse Pin";
	g_ColorNames[EColor::FloatLink] = "Float Pin";
	g_ColorNames[EColor::MatrixLink] = "Matrix Pin";
	g_ColorNames[EColor::QuatLink] = "Quaternion Pin";
	g_ColorNames[EColor::Vec3Link] = "Vector 3 Pin";
	g_ColorNames[EColor::Vec4Link] = "Vector 4 Pin";
	g_ColorNames[EColor::ScreenLink] = "Screen Pin";

	g_SizeNames[ESize::Nodes_Rounding] = "Nodes Rounding";
	g_SizeNames[ESize::Nodes_FloatWidth] = "Nodes Float Width";
	g_SizeNames[ESize::Nodes_FloatMargin] = "Nodes Float Margin";
	g_SizeNames[ESize::Nodes_BorderWidth] = "Nodes BorderWidth";

	g_SizeVecNames[ESizeVec2::Window_FramePadding] = "Windows Frame Padding";
	g_SizeVecNames[ESizeVec2::Nodes_IconSize] = "Nodes Icon Size";
	g_SizeVecNames[ESizeVec2::Nodes_PivotAlignment] = "Nodes Pivot Alignment";
	g_SizeVecNames[ESizeVec2::Nodes_PinSpacing] = "Nodes Pin Spacing";
	g_SizeVecNames[ESizeVec2::Nodes_ItemsSpacing] = "Nodes Items Spacing";
	g_SizeVecNames[ESizeVec2::Nodes_FloatPadding] = "Nodes Float Padding";
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
}
