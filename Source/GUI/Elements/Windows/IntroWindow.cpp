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
      LOG_DEBUG(entry.path().string())
      m_tutorial_headers.push_back(TutorialLoader::loadTutorialHeader(entry.path().string()));
    }
  }
}

void IntroWindow::render()
{
  bool tmp = true;
  ImGui::Begin("Intro", &tmp); //&Application::get().m_showIntroWindow
  for (auto& header : m_tutorial_headers) {
    if (ImGui::Button(header.m_title.c_str(), ImVec2(0,0))) {
      m_tutorial_window->setTutorial(header);  
    }
    ImGui::SameLine();
    ImGui::Text(header.m_filename.c_str());
    ImGui::Text(header.m_description.c_str());
    ImGui::Dummy(ImVec2(20, 5));
  }
  ImGui::End();
}

void IntroWindow::renderTutorials()
{
}
