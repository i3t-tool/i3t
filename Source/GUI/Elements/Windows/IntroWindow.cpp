#include "IntroWindow.h"
#include "Logger/Logger.h"
#include "Config.h"
#include "imgui.h"
#include "Utils/Other.h"
#include "Core/Application.h"
#include "Tutorial/TutorialLoader.h"


#include <filesystem>

IntroWindow::IntroWindow(bool show, TutorialWindow* tutorial_window) : IWindow(show)
{
  m_tutorial_window = tutorial_window;
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
  bool tmp = true;
  ImGui::Begin("Intro", &tmp); //&Application::get().m_showIntroWindow
  for (auto& header : m_tutorial_headers) {
    std::string buttonName = header->m_title + "##" + header->m_filename;
    if (ImGui::Button(buttonName.c_str(), ImVec2(0,0))) {
      m_tutorial_window->setTutorial(header);  
    }
    ImGui::SameLine();
    ImGui::Text(header->m_filename.c_str());
    ImGui::Text(header->m_description.c_str());
    ImGui::Dummy(ImVec2(20, 5));
  }
  ImGui::End();
}

void IntroWindow::renderTutorials()
{
}
