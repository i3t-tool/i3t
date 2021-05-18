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
  // load images
  try {
    dummyImage = std::make_shared<GUIImage>(GUIImage(FS::absolute(Config::TEXTURE_FOLDER) + "dummy.png"));
  }
  catch (std::runtime_error& e) {
    LOG_ERROR(e.what())
  }
  try {
    folderImage = std::make_shared<GUIImage>(GUIImage(FS::absolute(Config::TEXTURE_FOLDER) + "pilkaFolder.png"));
  }
  catch (std::runtime_error& e) {
    LOG_ERROR(e.what())
  }
  try {
    cvutImage = std::make_shared<GUIImage>(GUIImage(FS::absolute(Config::TEXTURE_FOLDER) + "cvutLogo.png"));
  }
  catch (std::runtime_error& e) {
    LOG_ERROR(e.what())
  }
  
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
  ImVec2 windowSize = ImVec2(1020, 600);
  static bool firstTime = true;
  if (firstTime) {
    firstTime = false;
    ImGui::SetNextWindowSize(windowSize);
  }
  const float leftBarWidth = 330;
  const float loadBtnWidth = 120;
  const float startNewBtnWidth = loadBtnWidth;
  const float thumbImageSize = 80;
  const float startBtnWidth = 120;

  ImGui::PushStyleColor(ImGuiCol_WindowBg , IM_COL32_WHITE);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
  ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 14);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0.8f * windowSize.x, 0.8f * windowSize.y));
  //ImGui::PushStyleColor(ImGuiCol_TitleBg, Application::get().getUI()->getTheme().get(EColor::TutorialBgColor));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, Application::get().getUI()->getTheme().get(EColor::TutorialBgColor));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, IM_COL32(202, 202, 202, 255));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, IM_COL32(202, 202, 202, 255));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, IM_COL32(202, 202, 202, 255));
  ImGui::PushStyleColor(ImGuiCol_Separator, IM_COL32(202, 202, 202, 255));
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(14, 98, 175, 255));
  ImGui::Begin("Intro", getShowPtr(), ImGuiWindowFlags_NoDocking); 
  {
    // LEFT CHILD WINDOW
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30, 30));
    ImGui::BeginChild("logo", ImVec2(leftBarWidth, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
    {
      // TITLE
      ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::I3TTitle));
      ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(14, 98, 175, 255));
      ImGui::Text("Learn\nTransformations");
      ImGui::PopStyleColor();
      ImGui::PopFont();

      ImGui::Dummy(ImVec2(0, 10));
      // DOTS
      float size = 10.f;
      float space = 7.f;
      ImVec2 screenPos = ImGui::GetCursorScreenPos();
      float x = screenPos.x;
      float y = screenPos.y;
      ImDrawList* draw_list = ImGui::GetWindowDrawList();
      for (int i = 0; i < 3; i++) {
        draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + size, y + size), IM_COL32(8, 187, 230, 255));
        x += size + space;
      }
      ImGui::Dummy(ImVec2(0, size));

      ImGui::Dummy(ImVec2(0, 10));
      // DESCRIPTION
      ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::I3TDescription));
      ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(65, 65, 66, 255));
      ImGui::TextWrapped("I3T is an educational application which enables the study of 3D transformation and their hierarchy in an illustrative way.");
      ImGui::PopStyleColor();
      ImGui::PopFont();

      // IMAGE
      if (cvutImage) {
        ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - cvutImage->m_height);
        ImGui::Image((ImTextureID)cvutImage->m_texID, ImVec2(cvutImage->m_width, cvutImage->m_height));
      }

      ImGui::EndChild();
    }
    ImGui::PopStyleVar();

    ImGui::SameLine();

    // RIGHT CHILD WINDOW
    ImGui::PushStyleColor(ImGuiCol_ChildBg , Application::get().getUI()->getTheme().get(EColor::TutorialBgColor));
    ImGui::BeginChild("panel", ImVec2(0, 0), false);
    {
      // WRAPPER CHILD WINDOW
      const ImVec2 outerPadding = ImVec2(10, 10);
      const ImVec2 innerPadding = ImVec2(10, 10);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, outerPadding);
      ImGui::BeginChild("wrapper", ImVec2(0, thumbImageSize + 2 * (outerPadding.y + innerPadding.y)), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
      {
        // SCENE PICKER CHILD WINDOW
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, innerPadding);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6);
        ImGui::PushStyleColor(ImGuiCol_ChildBg , IM_COL32_WHITE);
        ImGui::BeginChild("header", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
        {
          // FOLDER IMAGE
          if (folderImage) {
            //ImVec2 offset = ImVec2(ImGui::GetCursorPosX() + (thumbImageSize - folderImage->m_width)/2,ImGui::GetCursorPosY() + (thumbImageSize - folderImage->m_height)/2);
            //ImGui::Dummy(ImVec2(thumbImageSize, thumbImageSize));
            //ImGui::SetCursorPos(offset);
            ImGui::Image((ImTextureID)folderImage->m_texID, ImVec2(thumbImageSize, thumbImageSize));
          }
          else {
            
          }
          ImGui::SameLine();
          // YOUR SCENE TEXT
          //ImGui::AlignTextToFramePadding();
          ImGui::BeginGroup();
          {
            ImGui::BeginVertical("yourScene", ImVec2(0, 0));
            ImGui::Spring(1);  // not working

            ImGui::Dummy(ImVec2(0, 10));
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(65, 65, 66, 255));
            ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::IntroItemTitle));
            ImGui::Text("Your scene");
            ImGui::PopFont();
            ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::IntroItemDescription));
            ImGui::Text("Start with an empty scene or load your previous work.");
            ImGui::PopFont();
            ImGui::PopStyleColor();

            ImGui::Spring(1);  // not working
            ImGui::EndVertical();
            ImGui::EndGroup();
          }

          //ImGui::GetFontSize()
          //ImGui::SetNextItemWidth(100.0f);
        
          //const float itemSpacing = ImGui::GetStyle().
          ImGui::SameLine(ImGui::GetContentRegionMax().x - (loadBtnWidth + ImGui::GetStyle().WindowPadding.x + 5));
          // BUTTONS
          ImGui::BeginGroup();
          {
            ImGui::BeginVertical("buttons", ImVec2(0, 0));
            ImGui::Spring(1);

            ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::Button));
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(8, 187, 230, 255));
            if (ImGui::Button("New", ImVec2(startNewBtnWidth, 0))) {
              *this->getShowPtr() = false;
            }
            //ImGui::SameLine();
            ImGui::Dummy(ImVec2(0, 2));
            if (ImGui::Button("Load", ImVec2(loadBtnWidth, 0)))
            {
              // todo load from file
            }
            ImGui::PopStyleColor(2);
            ImGui::PopFont();

            ImGui::Spring(1);
            ImGui::EndVertical();
            ImGui::EndGroup();
          }
          ImGui::EndChild(); // SCENE PICKER
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);
        ImGui::EndChild(); // wrapper
      }
      ImGui::PopStyleVar();


      //ImGui::Spacing();


      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 0));
      //// TABS
      //if (ImGui::BeginTabBar("TabBar")) 
      //{
      //  // TUTORIALS
      //  if (ImGui::BeginTabItem("Tutorials")) 
      //  {
      // TUTORIAL LIST CHILD WINDOW
      ImGui::BeginChild("Tutorial list", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
      {
        //ImGui::Separator();
        for (auto& header : m_tutorial_headers) {
          // ITEM
          ImGui::BeginGroup();
          {
            const float titleDescWidth = ImGui::GetContentRegionAvail().x - (thumbImageSize + startBtnWidth);

            ImGui::Columns(3, "ThreeCols", false);
            ImGui::SetColumnWidth(0, thumbImageSize + 2 * ImGui::GetStyle().ColumnsMinSpacing);
            ImGui::SetColumnWidth(1, titleDescWidth - 2 * ImGui::GetStyle().ColumnsMinSpacing);
            //ImGui::SetColumnWidth(2, startBtnWidth);

            auto img = header->m_thumbnailImage;
            if (img) {
              ImGui::Image((ImTextureID)img->m_texID, ImVec2(thumbImageSize, thumbImageSize));
            }
            else {
              //todo load dummy at introwindow init
              if (dummyImage) {
                ImGui::Image((ImTextureID)dummyImage->m_texID, ImVec2(thumbImageSize, thumbImageSize));
              }
              else {
                ImGui::Image(nullptr, ImVec2(thumbImageSize, thumbImageSize));
              }
            }

            ImGui::NextColumn();
            //ImGui::SameLine();
            ImGui::BeginGroup();
            {
              // TITLE
              ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(14, 98, 175, 255));
              ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::IntroItemTitle));
              ImGui::TextWrapped(header->m_title.c_str());
              ImGui::PopStyleColor();
              ImGui::PopFont();
              // DESCRIPTIONS
              ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(65, 65, 66, 255));
              ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::IntroItemDescription));
              ImGui::TextWrapped(header->m_description.c_str());
              ImGui::PopStyleColor();
              ImGui::PopFont();
              ImGui::EndGroup();
            }

            ImGui::NextColumn();
            //ImGui::SameLine(ImGui::GetContentRegionMax().x - startNewBtnWidth - ImGui::GetStyle().WindowPadding.x);
            // START BUTTON
            ImGui::BeginGroup();
            {
              ImGui::BeginVertical("start button", ImVec2(0, thumbImageSize));
              ImGui::Spring(1);
              ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::Button));
              ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
              if (ImGui::Button("Start", ImVec2(startBtnWidth,0))) {
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
              ImGui::PopFont();
              ImGui::Spring(1);
              ImGui::EndVertical();
              ImGui::EndGroup();
            }
            ImGui::Columns(1); // end columns
            ImGui::Dummy(ImVec2(0, 2));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 2));
            ImGui::EndGroup();
          }
          // ITEM ACTIONS
          if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("yay");
            ImGui::EndTooltip();
          }
        }
        ImGui::EndChild(); // list
      }
      //    ImGui::EndTabItem();
      //  }
      //  // TEMPLATES
      //  if (ImGui::BeginTabItem("Templates")) 
      //  {
      //    ImGui::Text("List of preset scene files");
      //    ImGui::EndTabItem();
      //  }
      //  // RECENT
      //  if (ImGui::BeginTabItem("Recent")) 
      //  {
      //    ImGui::Text("List of recent files files");
      //    ImGui::EndTabItem();
      //  }
      //  ImGui::EndTabBar();
      //}
      ImGui::PopStyleVar();
      ImGui::EndChild(); // right panel
    }
    ImGui::PopStyleColor();

    ImGui::End(); // window
  }
  ImGui::PopStyleVar(5);
  ImGui::PopStyleColor(7);

}

void IntroWindow::renderTutorials()
{
}
