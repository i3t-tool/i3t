#include "IntroWindow.h"

#include <filesystem>

#include "imgui.h"

#include "Config.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include "Commands/ApplicationCommands.h"
#include "Logger/Logger.h"
#include "Tutorial/TutorialLoader.h"
#include "Utils/Other.h"

IntroWindow::IntroWindow(bool show) : IWindow(show)
{
  // load images
  try {
    m_dummyImage = std::make_shared<GUIImage>(GUIImage(Config::getAbsolutePath(Config::TEXTURE_FOLDER) + "dummy.png"));
  }
  catch (std::runtime_error& e) {
    LOG_ERROR(e.what())
  }
  try {
    m_folderImage = std::make_shared<GUIImage>(GUIImage(Config::getAbsolutePath(Config::TEXTURE_FOLDER) + "pilkaFolder.png"));
  }
  catch (std::runtime_error& e) {
    LOG_ERROR(e.what())
  }
  try {
    m_cvutImage = std::make_shared<GUIImage>(GUIImage(Config::getAbsolutePath(Config::TEXTURE_FOLDER) + "cvutLogo.png"));
  }
  catch (std::runtime_error& e) {
    LOG_ERROR(e.what())
  }
  try {
    m_i3tImage = std::make_shared<GUIImage>(GUIImage(Config::getAbsolutePath(Config::TEXTURE_FOLDER) + "logoi3t.png"));
  }
  catch (std::runtime_error& e) {
    LOG_ERROR(e.what())
  }
  LOG_DEBUG("debug")
  LOG_INFO("info");
  LOG_ERROR("info");
  Log::debug("debug new");
  Log::info("info new");
  Log::fatal("fatal new");
  reloadTutorials();
}

void IntroWindow::reloadTutorials()
{
  // preload all tutorials located in TUTORIALS_FOLDER recursively
  auto path = std::filesystem::path(Config::getAbsolutePath(Config::TUTORIALS_FOLDER));
  path.make_preferred(); // unifies the directory separator for this platform
  LOG_INFO("Searching for tutorials in: " + path.string())
  auto dir_iterator = std::filesystem::recursive_directory_iterator(path);
  for (auto& entry : dir_iterator) {
    if (entry.path().extension() == ".tut") {
      std::string pathString = entry.path().string();
      LOG_DEBUG(pathString)
    	// Load header part of tutorial
      if (std::shared_ptr<TutorialHeader> header = TutorialLoader::loadTutorialHeader(pathString); header) {
        m_tutorial_headers.push_back(std::move(header));
      }
      else {
        LOG_ERROR("Tutorial header " + pathString + " not loaded." );
      }
    }
  }
}

/**
 * @brief Renders the whole tutorial window
*/
void IntroWindow::render()
{
	// Set initial window size
  ImVec2 windowSize = ImVec2(1020, 600);
  static bool firstTime = true;
  if (firstTime) {
    firstTime = false;
    ImGui::SetNextWindowSize(windowSize);
  }
	// Styling constants
	const ImVec2 logoOffset = ImVec2(5, -20);
	const float titleVerticalOffset = 130;
  const float leftBarWidth = 330;
  const float loadBtnWidth = 120;
  const float startNewBtnWidth = loadBtnWidth;
  const float thumbImageSize = 80;
  const float startBtnWidth = 120;

	// WINDOW
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
  ImGui::Begin("##Intro", getShowPtr(), ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse); 
  {
    // LOGO I3T
    ImVec2 logoPos = ImGui::GetWindowPos() + logoOffset;
  	ImGui::GetForegroundDrawList()->AddImage((ImTextureID)m_i3tImage->m_texID, logoPos, logoPos + ImVec2(m_i3tImage->m_width, m_i3tImage->m_height));
    // LEFT CHILD WINDOW
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30, 30));
    ImGui::BeginChild("logo", ImVec2(leftBarWidth, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
    {
      ImGui::Dummy(ImVec2(0, titleVerticalOffset));
    	
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
      if (m_cvutImage) {
        ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - m_cvutImage->m_height);
        ImGui::Image((ImTextureID)m_cvutImage->m_texID, ImVec2(m_cvutImage->m_width, m_cvutImage->m_height));
      }

      ImGui::EndChild();
    }
    ImGui::PopStyleVar();

    ImGui::SameLine();

    // RIGHT CHILD WINDOW
    
    //ImGui::Spacing();
    //// TABS
    //if (ImGui::BeginTabBar("TabBar")) 
    //{
    //  // TUTORIALS
    //  if (ImGui::BeginTabItem("Tutorials")) 
    //  {
    //    ---
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
      
    const ImVec2 outerPadding = ImVec2(10, 10);
    const ImVec2 innerPadding = ImVec2(10, 10);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, outerPadding);
    ImGui::PushStyleColor(ImGuiCol_ChildBg , Application::get().getUI()->getTheme().get(EColor::TutorialBgColor));
    ImGui::BeginChild("panel", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
    {

	    // SCENE PICKER CHILD WINDOW
	    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, innerPadding);
	    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6);
	    ImGui::PushStyleColor(ImGuiCol_ChildBg , IM_COL32_WHITE);
	    ImGui::BeginChild("header", ImVec2(0, thumbImageSize + 2*innerPadding.y), true, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
	    {
		    // FOLDER IMAGE
		    if (m_folderImage) {
			    //ImVec2 offset = ImVec2(ImGui::GetCursorPosX() + (thumbImageSize - folderImage->m_width)/2,ImGui::GetCursorPosY() + (thumbImageSize - folderImage->m_height)/2);
			    //ImGui::Dummy(ImVec2(thumbImageSize, thumbImageSize));
			    //ImGui::SetCursorPos(offset);
			    ImGui::Image((ImTextureID)m_folderImage->m_texID, ImVec2(thumbImageSize, thumbImageSize));
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
		    ImGui::SameLine(ImGui::GetContentRegionMax().x - loadBtnWidth);
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

      ImGui::Dummy(ImVec2(0, 5));
    	
      // TUTORIAL LIST
      for (auto& header : m_tutorial_headers)
        {
          // ITEM
          ImGui::BeginGroup();
          {
	          ImGui::Indent(innerPadding.x);
            const float titleDescWidth = ImGui::GetContentRegionAvail().x - (thumbImageSize + startBtnWidth);

            //ImGui::Columns(3, "ThreeCols", false);
            //ImGui::SetColumnWidth(0, thumbImageSize + 2 * ImGui::GetStyle().ColumnsMinSpacing);
            //ImGui::SetColumnWidth(1, titleDescWidth - 2 * ImGui::GetStyle().ColumnsMinSpacing);
            //ImGui::SetColumnWidth(2, startBtnWidth);

            auto img = header->m_thumbnailImage;
            if (img) {
              ImGui::Image((ImTextureID)img->m_texID, ImVec2(thumbImageSize, thumbImageSize));
            }
            else {
              //todo load dummy at introwindow init
              if (m_dummyImage) {
                ImGui::Image((ImTextureID)m_dummyImage->m_texID, ImVec2(thumbImageSize, thumbImageSize));
              }
              else {
                ImGui::Image(nullptr, ImVec2(thumbImageSize, thumbImageSize));
              }
            }

            //ImGui::NextColumn();
            //ImGui::SameLine(ImGui::GetCursorPosX() + innerPadding.x);
            ImGui::SameLine();
          	std::string descChildName = "Desc##" + header->m_filename;
          	ImVec2 descSize (ImGui::GetContentRegionAvailWidth(), thumbImageSize);
	          descSize.x -= (startNewBtnWidth + 2*innerPadding.x + outerPadding.x);
          	//ImGui::PushClipRect(ImGui::GetCursorScreenPos(), descBottomRight, true);
            //ImGui::BeginGroup();
          	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
          	ImGui::BeginChild(descChildName.c_str(), descSize, false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
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
              //ImGui::EndGroup();
	          	//ImGui::PopClipRect();
	          	ImGui::EndChild();
            }
          	ImGui::PopStyleVar();

            //ImGui::NextColumn();
            ImGui::SameLine(ImGui::GetContentRegionMax().x - startNewBtnWidth - innerPadding.x - outerPadding.x);
            // START BUTTON
            ImGui::BeginGroup();
            {
              ImGui::BeginVertical("start button", ImVec2(0, thumbImageSize));
              ImGui::Spring(1);
              ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::Button));
              ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
            	std::string buttonName = "Start##" + header->m_filename;
              if (ImGui::Button(buttonName.c_str(), ImVec2(startBtnWidth,0))) {
              	// TUTORIAL LOADING !!!
                auto tutorial = TutorialLoader::loadTutorial(header);
                if (tutorial) {
                  Log::debug("Tutorial " + header->m_title + " loaded");
                  SetTutorialCommand::dispatch(tutorial);
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
            //ImGui::Columns(1); // end columns
            ImGui::Dummy(ImVec2(0, 2));
          	// CUSTOM SEPARATOR
	          float thickness_draw = 1.0f;
	          float thickness_layout = 0.0f;
          	ImVec2 sceenPos = ImGui::GetCursorScreenPos();
	          const ImRect bb(sceenPos, ImVec2(sceenPos.x + ImGui::GetContentRegionAvailWidth() - innerPadding.x, sceenPos.y + thickness_draw));
	          ImGui::ItemSize(ImVec2(0.0f, thickness_layout));
	          if (ImGui::ItemAdd(bb, 0))
	          {
		          // Draw
	          	ImGui::GetWindowDrawList()->AddLine(bb.Min, ImVec2(bb.Max.x, bb.Min.y), ImGui::GetColorU32(ImGuiCol_Separator));
	          }
          	//ImGui::Separator();
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

      ImGui::EndChild(); // right panel
    }
    ImGui::PopStyleColor();
  	ImGui::PopStyleVar();

    ImGui::End(); // window
  }
  ImGui::PopStyleVar(5);
  ImGui::PopStyleColor(7);

}

void IntroWindow::renderTutorials()
{
}
