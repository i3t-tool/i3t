#include "Theme.h"

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

  m_fontsAssoc.insert(std::pair(EFont::MenuLarge, 0));
  m_fontsAssoc.insert(std::pair(EFont::TutorialText, 0));
  m_fontsAssoc.insert(std::pair(EFont::Button, 1));
  m_fontsAssoc.insert(std::pair(EFont::Tab, 1));
  m_fontsAssoc.insert(std::pair(EFont::Node, 2));
  m_fontsAssoc.insert(std::pair(EFont::MenuSmall, 2));
  m_fontsAssoc.insert(std::pair(EFont::Title, 3));
  m_fontsAssoc.insert(std::pair(EFont::TaskTitle, 4));
}

void Theme::init()
{
  auto& io = ImGui::GetIO();

  m_fonts = {
      io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("/Data/fonts/Roboto-Regular.ttf").c_str(),
                                   14.0f * m_fontScale),
      io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("/Data/fonts/Roboto-Bold.ttf").c_str(), 12.0f * m_fontScale),
      io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("/Data/fonts/Roboto-Regular.ttf").c_str(),
                                   12.0f * m_fontScale),
      io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("/Data/fonts/Ubuntu-Bold.ttf").c_str(), 24.0f * m_fontScale),
      io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("/Data/fonts/Roboto-Bold.ttf").c_str(), 14.0f * m_fontScale),
  };
	io.FontDefault = get(EFont::MenuLarge);
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

  style.FramePadding.y = 8.0f;
}
