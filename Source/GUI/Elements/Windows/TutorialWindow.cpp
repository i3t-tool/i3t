#include "TutorialWindow.h"

#include <utility>

#include <imgui.h>

/// \todo If you want to use ImGui Markdown run $ git submodule add https://github.com/juliettef/imgui_markdown.git
/// Depencencies/imgui_markdown. After that modify CMakeLists.txt, append
/// include_directories(Dependencies/imgui_markdown) to block of includes.
// #include <ImGui_markdown.h>

#include "Config.h"
#include "Core/Application.h"
#include "Tutorial/Tutorial.h"

TutorialWindow::TutorialWindow(bool show) : IWindow(show)
{
  m_tutorial = nullptr;
  m_current_step = 0;

  // TEMPORARY
  setTutorial(Config::getAbsolutePath("/Data/tutorials/test_quite_simple.yml"));
}

void TutorialWindow::setTutorial(std::string filename)
{
  delete m_tutorial;
  m_tutorial = new Tutorial(std::move(filename));
  m_current_step = 0;
}

void TutorialWindow::render()
{
  if (!Application::get().m_showTutorialWindow)
    return;

  // SET STYLE
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 20.0f));
  ImGui::Begin("Tutorial window", &Application::get().m_showTutorialWindow);

  // CREATE IMGUI CONTENT
  renderTutorialContent();
  renderTutorialControls();

  ImGui::PopStyleVar(1);
  ImGui::End();
}

void TutorialWindow::renderTutorialContent()
{
  ImGui::Text(u8"かきくけこéíšířáěéšíčřá");

  if (m_tutorial)
  {
    m_tutorial->render(m_current_step);
  }
  else
  {
    // default content
    ImGui::PushFont(App::get().getFont(FONT_TITLE));
    ImGui::Text("Model transformations");
    ImGui::PopFont();
    ImGui::Spacing();

    ImGui::PushFont(App::get().getFont(FONT_TUTORIAL_TEXT));
    ImGui::PushTextWrapPos(0.0f);
    ImGui::Text("The scene demonstrates an ");
    ImGui::SameLine(0.0);
    ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), "application");
    ImGui::SameLine(0.0);
    ImGui::Text(" of the model transformation that is commonly used to define "
                "the position, the orientation, and the scale of a geometric object in the worlds coordinates.");
    ImGui::PopTextWrapPos();
    ImGui::PopFont();

    ImGui::PushFont(App::get().getFont(FONT_TASK_TITLE));
    ImGui::Text("Assignment 3 of 12:");
    ImGui::PopFont();

    ImGui::PushFont(App::get().getFont(FONT_TUTORIAL_TEXT));
    ImGui::TextWrapped(
        "Try to change the parameters of the given transformations and explain the behaviour of each of them");
    ImGui::PopFont();
  }
}

void TutorialWindow::renderTutorialControls()
{
  ImGui::NewLine();
  // ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0,0,0,1));
  ImGui::ProgressBar(0.3f, ImVec2(-1, 20));
  // ImGui::PopStyleColor();

  ImGui::NewLine();
  if (ImGui::Button("Back", ImVec2(100, 0)))
  {
    if (m_current_step != 0)
      m_current_step--;
  }
  ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 100);
  if (ImGui::Button("Next", ImVec2(100, 0)))
  {
    if (m_current_step != m_tutorial->getNumSteps() - 1)
      m_current_step++;
  }
}
