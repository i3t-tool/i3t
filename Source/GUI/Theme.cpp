#include "Theme.h"

#include "imgui_node_editor.h"

#include "Config.h"

Theme::Theme()
{
	set(EColor::Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	set(EColor::WindowBackground, ImVec4(0.439f, 0.439f, 0.455f, 1.00f));
	set(EColor::PopupBackground, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	set(EColor::MenuBarBackground, ImVec4(0.40f, 0.38f, 0.369f, 1.00f));
	set(EColor::Border, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	set(EColor::PrimaryColor, ImVec4(0.278f, 0.278f, 0.286f, 1.00f));
	set(EColor::ActiveColor, ImVec4(0.259f, 0.588f, 0.980f, 1.00f));

	set(EColor::TutorialBgColor,ImVec4(232.0f / 255.0f, 232.0f / 255.0f, 232.0f / 255.0f, 1.00f) );

	set(EColor::PulseLink, ImVec4(1.009f, 1.00f, 1.00f, 1.00f));
	set(EColor::FloatLink, ImVec4(1.009f, 1.00f, 1.00f, 1.00f));
	set(EColor::MatrixLink, ImVec4(68.0f / 255.0f, 201.0f / 255.0f, 156.0f / 255.0f, 1.00f));
	set(EColor::QuatLink, ImVec4(124.0f / 255.0f, 21.0f / 255.0f, 153.0f / 255.0f, 1.00f));
	set(EColor::Vec3Link, ImVec4(218.0f / 255.0f, 0.0f / 255.0f, 183.0f / 255.0f, 1.00f));
	set(EColor::Vec4Link, ImVec4(218.0f / 255.0f, 0.0f / 255.0f, 183.0f / 255.0f, 1.00f));
	set(EColor::ScreenLink, ImVec4(218.0f / 255.0f, 0.0f / 255.0f, 183.0f / 255.0f, 1.00f));

	m_fontsAssoc.insert(std::pair(EFont::MenuLarge, 0));
	m_fontsAssoc.insert(std::pair(EFont::TutorialText, 0));
	m_fontsAssoc.insert(std::pair(EFont::Button, 1));
	m_fontsAssoc.insert(std::pair(EFont::Tab, 1));
	m_fontsAssoc.insert(std::pair(EFont::Node, 2));
	m_fontsAssoc.insert(std::pair(EFont::MenuSmall, 2));
	m_fontsAssoc.insert(std::pair(EFont::Title, 3));
	m_fontsAssoc.insert(std::pair(EFont::TaskTitle, 4));
	m_fontsAssoc.insert(std::pair(EFont::Header, 5));

	m_sizes[static_cast<size_t>(ESize::Nodes_FloatMargin)] = 1.0f;
	m_sizes[static_cast<size_t>(ESize::Nodes_FloatWidth)] = 60.0f;
	m_sizes[static_cast<size_t>(ESize::Nodes_Rounding)] = 4.0f;

	m_sizes[static_cast<size_t>(ESize::Nodes_ItemsSpacingX)] = 4.0f;
	m_sizes[static_cast<size_t>(ESize::Nodes_ItemsSpacingY)] = 4.0f;

	m_sizes[static_cast<size_t>(ESize::Window_FramePadding)] = 8.0f;
}

void Theme::init()
{
	auto& io = ImGui::GetIO();

	const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0080, 0x07FF, // Czech 
		0,
	};

	m_fonts = {
			io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("Data/fonts/Roboto-Regular.ttf").c_str(),
	                                 14.0f * m_fontScale, nullptr, ranges),
			io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("Data/fonts/Roboto-Bold.ttf").c_str(),
	                                 12.0f * m_fontScale, nullptr, ranges),
			io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("Data/fonts/Roboto-Regular.ttf").c_str(),
	                                 12.0f * m_fontScale, nullptr, ranges),
			io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("Data/fonts/Ubuntu-Bold.ttf").c_str(),
	                                 24.0f * m_fontScale, nullptr, ranges),
			io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("Data/fonts/Roboto-Bold.ttf").c_str(),
	                                 14.0f * m_fontScale, nullptr, ranges),
      io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("Data/fonts/Roboto-Bold.ttf").c_str(),
                                   20.0f * m_fontScale, nullptr, ranges),
	};
	io.FontDefault = get(EFont::MenuLarge);
	io.Fonts->Build();
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

	style.FramePadding.x = 8.0f;
	style.FramePadding.y = m_sizes[static_cast<size_t>(ESize::Window_FramePadding)];
	style.TabRounding = 2.0f;
	style.ChildBorderSize = 0.0f;
	style.PopupBorderSize = 0.0f;
	style.WindowBorderSize = 0.0f;

	auto& nodesStyle = ax::NodeEditor::GetStyle();
	nodesStyle.NodeRounding = m_sizes[static_cast<size_t>(ESize::Nodes_Rounding)];
	nodesStyle.NodeBorderWidth = 0.0f;
}
