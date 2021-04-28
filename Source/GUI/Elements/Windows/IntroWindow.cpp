#include "IntroWindow.h"
#include "Logger/Logger.h"
#include "Config.h"
#include "imgui.h"
#include "Commands/ApplicationCommands.h"
#include "Utils/Other.h"
#include "Core/Application.h"
#include "Tutorial/TutorialLoader.h"


#include <filesystem>

IntroWindow::IntroWindow(bool show) : IWindow(show)
{
  reloadTutorials();
}

void IntroWindow::reloadTutorials()
{
  // preload all tutorials located in TUTORIALS_FOLDER and its subfolders
  auto path = std::filesystem::path(FS::absolute(Config::TUTORIALS_FOLDER));
  path.make_preferred(); // unifies the directory separator for this platform
  LOG_INFO("Searching for tutorials in: " + path.string())
  auto dir_iterator = std::filesystem::recursive_directory_iterator(path);
  for (auto& entry : dir_iterator) {
    if (entry.path().extension() == ".tut") {
      std::string pathString = entry.path().string();
      LOG_DEBUG(pathString)
      std::optional<std::shared_ptr<TutorialHeader>> header = TutorialLoader::loadTutorialHeader(pathString);
      if (header.has_value()) {
        m_tutorial_headers.push_back(header.value());
      }
      else {
        LOG_ERROR("Tutorial header " + pathString + " not loaded." );
      }
    }
  }
}

void IntroWindow::render()
{
  ImGui::PushStyleColor(ImGuiCol_WindowBg , IM_COL32_WHITE);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
  ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 20);
  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(51, 51, 51, 255));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, IM_COL32(215, 215, 215, 255));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, IM_COL32(232, 232, 232, 255));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, IM_COL32(240, 240, 240, 255));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, IM_COL32(245, 245, 245, 255));
  ImGui::SetNextWindowSize(ImVec2(800, 500));
  ImGui::Begin("Intro", getShowPtr(), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking); 
  {
    // LEFT
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
    ImGui::BeginChild("logo", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.36f, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
    {
      // CONTENT
      ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::Title));
      ImGui::PushStyleColor(ImGuiCol_Text, Application::get().getUI()->getTheme().get(EColor::ActiveColor));
      ImGui::TextWrapped("Learn Transformations");
      ImGui::PopStyleColor();
      ImGui::PopFont();

      ImGui::Spacing();

      ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::Tab));
      ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32_BLACK);
      ImGui::TextWrapped("I3T is an educational application which enables the study of 3D transformation and their hierarchy in an illustrative way.");
      ImGui::PopStyleColor();
      ImGui::PopFont();

      ImGui::EndChild();
    }
    ImGui::PopStyleVar();

    ImGui::SameLine();

    // RIGHT
    ImGui::PushStyleColor(ImGuiCol_ChildBg , Application::get().getUI()->getTheme().get(EColor::TutorialBgColor));
    ImGui::BeginChild("panel", ImVec2(0, 0), false);
    { 
      // HEADER
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
      ImGui::BeginChild("header", ImVec2(0, 80), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
      {
        ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::Header));
        ImGui::PushStyleColor(ImGuiCol_Text, Application::get().getUI()->getTheme().get(EColor::PrimaryColor));
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Choose file");
        ImGui::PopStyleColor();
        ImGui::PopFont();

        //ImGui::GetFontSize()
        //ImGui::SetNextItemWidth(100.0f);

        const float loadBtnWidth = 80;
        const float startBtnWidth = 120;
        const float itemSpacing = ImGui::GetStyle().ItemSpacing.x;
        //const float itemSpacing = ImGui::GetStyle().
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - (loadBtnWidth + startBtnWidth + itemSpacing));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
        if (ImGui::Button("Load", ImVec2(loadBtnWidth, 0)))
        {
          // todo load from file
        }
        ImGui::SameLine();
        if (ImGui::Button("Start new", ImVec2(startBtnWidth, 0))) {
          *this->getShowPtr() = false;
        }
        ImGui::PopStyleColor();
        ImGui::EndChild(); // header
      }
      ImGui::PopStyleVar();

      ImGui::Spacing();
      // LIST
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 0));
      ImGui::BeginChild("list", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
      {
        for (auto& header : m_tutorial_headers) {
          // ITEM
          ImGui::BeginGroup();
          {
            auto img = header->m_thumbnailImage;
            if (img) {
              ImGui::Image((ImTextureID)img->m_texID, ImVec2(50, 50));
            }
            else {
              //todo load dummy at introwindow init
              ImGui::Image(nullptr, ImVec2(50, 50));
            }
            ImGui::SameLine();
            std::string buttonName = header->m_title + "##" + header->m_filename;
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
            if (ImGui::Button(buttonName.c_str(), ImVec2(0,0))) {
              auto tutorial = TutorialLoader::loadTutorial(header);
              if (tutorial.has_value()) {
                Log::debug("Tutorial " + header->m_title + " loaded");
                SetTutorialCommand::dispatch(tutorial.value());
              }
              else {
                Log::info("ERR: Tutorial " + header->m_title + " not loaded");
              }
            }
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::Text(header->m_filename.c_str());
            ImGui::Text(header->m_description.c_str());
            ImGui::Dummy(ImVec2(20, 5));
          }
          ImGui::EndGroup();
          // ITEM ACTIONS
          if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("yay");
            ImGui::EndTooltip();
          }
        }
        ImGui::EndChild(); // list
      }
      ImGui::PopStyleVar();
      
      ImGui::EndChild(); // panel
    }
    ImGui::PopStyleColor();

    ImGui::End(); // window
  }
  ImGui::PopStyleVar(3);
  ImGui::PopStyleColor(6);

}

void IntroWindow::renderTutorials()
{
}
