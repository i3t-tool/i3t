#include "TutorialWindow.h"

#include <utility>

#include <imgui.h>

/// \todo If you want to use ImGui Markdown run $ git submodule add https://github.com/Mizumaky/imgui_markdown_extended
/// Depencencies/imgui_markdown. After that modify CMakeLists.txt, append
/// include_directories(Dependencies/imgui_markdown) to block of includes.
/// a taky zmenit v nem pridani do filtru ve VS a asi i dalsi veci - jeste se zeptat!
#include <imgui_markdown.h>

#include "Config.h"
#include "Core/Application.h"
#include "Tutorial/Tutorial.h"
#include "Tutorial/TutorialLoader.h"

// TUTORIAL GUI PROPERTIES DEFINITIONS
//---

const int NEXT_BUTTON_SIZE_X = 150;
const int SIMPLE_SPACE = 10;
const int CONTROLS_SIZE_Y = 100;


// TUTORIAL WINDOW FUNCTIONS
//---

TutorialWindow::TutorialWindow(bool show) : IWindow(show)
{
  m_tutorial = nullptr;
  m_current_step = 0;

  // TEMPORARY todo
  setTutorial(Config::getAbsolutePath("/Data/tutorials/test_v1.tut"));
}

void TutorialWindow::setTutorial(std::string filename)
{
  m_tutorial = TutorialLoader::loadFile(std::move(filename)); // btw if there was a previous unique pointer to another Tutorial, it gets deleted at this reassignment (yay, thats why we are using it! \^^/)
  setStep(0);
}

bool TutorialWindow::setStep(int step_number)
{
  if (m_tutorial == nullptr || step_number < 0 || step_number < m_tutorial->getStepCount())
  {
    return false;
  }
  m_current_step = step_number;
  return true;
}

//---

void TutorialWindow::render()
{
  if (!Application::get().m_showTutorialWindow)
    return;

  // PUSH STYLE 
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30.0f, 35.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
  ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 20);
  ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(232, 232, 232, 255));
  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(51, 51, 51, 255));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, IM_COL32(215, 215, 215, 255));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, IM_COL32(232, 232, 232, 255));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, IM_COL32(240, 240, 240, 255));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, IM_COL32(245, 245, 245, 255));
  ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(66, 150, 250, 255));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(66, 150, 250, 205));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(66, 150, 250, 102));
  //ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(245, 245, 245, 255));
  // BEGIN WINDOW
  const std::string window_name = "Tutorial - " + m_tutorial->m_title + "###Tutorial window";
  ImGui::Begin(window_name.c_str(), &Application::get().m_showTutorialWindow);

  // CREATE IMGUI CONTENT
  renderTutorialHeader();
  renderTutorialContent();
  renderTutorialControls();

  // POP STYLE
  ImGui::PopStyleVar(3);
  ImGui::PopStyleColor(9);
  // END WINDOW
  ImGui::End();
}

void TutorialWindow::renderTutorialHeader()
{
  //ImGui::BeginChild("header");
  std::string title;
  if (m_tutorial->getStepCount() > 0) {
    title = m_tutorial->m_steps[m_current_step].m_title;
  }
  else {
    title = "Empty tutorial";
  }

  ImGui::PushFont(App::get().getFont(FONT_TITLE));
  ImGui::TextWrapped(title.c_str());
  ImGui::PopFont();
  ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE)); // vertical spacing
  //ImGui::EndChild();
}

void TutorialWindow::renderTutorialContent()
{
  // PUSH STYLE
  //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  // BEGIN CHILD
  ImGui::BeginChild("content", ImVec2(0, -(CONTROLS_SIZE_Y + ImGui::GetStyle().ItemSpacing.y))); // be of size: [remaining Y space] - [planned Y size of contents]
  // btw ImGui::GetContentRegionAvail().y - remaining space Y 
  //ImGui::PopStyleVar();

  // SET UP MARKDOWN
    //    ImGuiIO& io = ImGui::GetIO();
  //io.Fonts->Clear();
  //// Base font
  //io.Fonts->AddFontFromFileTTF( "myfont.ttf", fontSize_ );
  //// Bold headings H2 and H3
  //H2 = io.Fonts->AddFontFromFileTTF( "myfont-bold.ttf", fontSize_ );
  //H3 = mdConfig.headingFormats[ 1 ].font;
  //// bold heading H1
  //float fontSizeH1 = fontSize_ * 1.1f;
  //H1 = io.Fonts->AddFontFromFileTTF( "myfont-bold.ttf", fontSizeH1 );
  const ImGui::MarkdownConfig mdConfig{nullptr, nullptr, nullptr, "link", { { nullptr, true }, { nullptr, true }, { nullptr, false } }, nullptr };

  if (m_tutorial)
  {
    if (m_tutorial->getStepCount() > 0) {
      // ITERATE OVER STEPS
      for (std::unique_ptr<TWidget>& widget_uptr : m_tutorial->m_steps[m_current_step].m_content) {
        const TWidget* const widget = widget_uptr.get(); // tenhle pointer se nebude nestarat o konstrukci/destrukci a ani (bonusove) o zmenu toho objektu -> its okay to have it (zaruceno constantami) https://stackoverflow.com/questions/10802046/whats-the-point-of-stdunique-ptrget

        if (const TWText* const tw_text = dynamic_cast<const TWText*>(widget); tw_text != nullptr) {
          Markdown( tw_text->m_text.c_str(), tw_text->m_text.length(), mdConfig);
        }
        else if (const TWSpacing* const tw_spacing = dynamic_cast<const TWSpacing*>(widget); tw_spacing != nullptr) {
          ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE)); // vertical spacing
        }
        else if (const TWTask* const tw_task = dynamic_cast<const TWTask*>(widget); tw_task != nullptr) {
          ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE)); 
          ImGui::PushFont(App::get().getFont(FONT_TASK_TITLE));
          ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(66, 150, 250, 255));

          ImGui::TextWrapped(tw_task->m_task.c_str());

          ImGui::PopStyleColor();
          ImGui::PopFont();
          ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE));
        }
        else if (const TWHint* const tw_hint = dynamic_cast<const TWHint*>(widget); tw_hint != nullptr) {
          ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
          if (ImGui::CollapsingHeader("Napoveda")) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(66, 150, 250, 255));
            ImGui::TextWrapped(tw_hint->m_hint.c_str());
            ImGui::PopStyleColor();
          }
          ImGui::PopStyleColor();
        }
        else {
          std::cout << "wrong widget type" << std::endl;
        }
      
      }
    }

    //    const std::string markdownText = u8R"(
    //# H1 Header: Text and Links
    //You can add [links like this one to enkisoftware](https://www.enkisoftware.com/) and lines will wrap well.
    //## H2 Header: indented text.
    //  This text has an indent (two leading spaces).
    //  This one has two.
    //### H3 Header: Lists
    //  * Unordered lists
    //  * Lists can be indented with two extra spaces.
    //  * Lists can have [links like this one to Avoyd](https://www.avoyd.com/)
    //)";

    //ImGui::Text(u8"かきくけこéíšířáěéšíčřá");
    //for (const TWidget& widget : m_tutorial->m_steps[m_current_step].m_content)
    //{
    //  if (widget.m_type == "text")
    //  {
    //    ImGui::PushFont(App::get().getFont(FONT_TUTORIAL_TEXT));
    //    ImGui::TextWrapped(("Text: " + widget.m_string).c_str());
    //    ImGui::PopFont();
    //  }
    //  else if (widget.m_type == "h1")
    //  {
    //    ImGui::PushFont(App::get().getFont(FONT_TITLE));
    //    ImGui::TextWrapped(("H1: " + widget.m_string).c_str());
    //    ImGui::PopFont();
    //  }
    //  else if (widget.m_type == "h2")
    //  {
    //    ImGui::PushFont(App::get().getFont(FONT_TITLE)); // todo
    //    ImGui::TextWrapped(("H2: " + widget.m_string).c_str());
    //    ImGui::PopFont();
    //  }
    //  else if (widget.m_type == "img")
    //  {
    //    ImGui::PushFont(App::get().getFont(FONT_TUTORIAL_TEXT));
    //    ImGui::TextWrapped(("Img: " + widget.m_string).c_str());
    //    ImGui::PopFont();
    //    ImGui::Image((void*)(intptr_t)getImageID(widget.m_string), ImVec2(256, 256));
    //  }
    //  else if (widget.m_type == "task")
    //  {
    //    ImGui::PushFont(App::get().getFont(FONT_TASK_TITLE));
    //    ImGui::TextWrapped(("Task: " + widget.m_string).c_str());
    //    ImGui::PopFont();
    //  }
    //  else if (widget.m_type == "hint")
    //  {
    //    ImGui::PushFont(App::get().getFont(FONT_TUTORIAL_TEXT));
    //    ImGui::TextWrapped(("Hint: " + widget.m_string).c_str());
    //    ImGui::PopFont();
    //  }
    //  else
    //  {
    //    ImGui::PushFont(App::get().getFont(FONT_TUTORIAL_TEXT));
    //    ImGui::TextWrapped(("Unknown type: " + widget.m_string).c_str());
    //    ImGui::PopFont();
    //  }
    //  ImGui::Spacing();
    //}
  }
  else
  {
    // default content
    // todo eg "No tutorial loaded. You can close the window or choose one here:"
    
    // ImGui::PushFont(App::get().getFont(FONT_TITLE));
    // ImGui::Text("Model transformations");
    // ImGui::PopFont();
    // ImGui::Spacing(); //TODO change my spacing into the default imgui one
    //
    // ImGui::PushFont(App::get().getFont(FONT_TUTORIAL_TEXT));
    // ImGui::PushTextWrapPos(0.0f);
    // ImGui::Text("The scene demonstrates an ");
    // ImGui::SameLine(0.0);
    // ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), "application");
    // ImGui::SameLine(0.0);
    // ImGui::Text(" of the model transformation that is commonly used to define "
    //             "the position, the orientation, and the scale of a geometric object in the worlds coordinates.");
    // ImGui::PopTextWrapPos();
    // ImGui::PopFont();
    //
    // ImGui::PushFont(App::get().getFont(FONT_TASK_TITLE));
    // ImGui::Text("Assignment 3 of 12:");
    // ImGui::PopFont();
    //
    // ImGui::PushFont(App::get().getFont(FONT_TUTORIAL_TEXT));
    // ImGui::TextWrapped(
    //     "Try to change the parameters of the given transformations and explain the behaviour of each of them");
    // ImGui::PopFont();
  }
  ImGui::EndChild();
}

void TutorialWindow::renderTutorialControls()
{
  // PUSH STYLE
  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
  // BEGIN CHILD
  ImGui::BeginChild("controls", ImVec2(0,0)); // stretch remaining Y space
  ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE)); // vertical spacing

  // ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0,0,0,1));
  ImGui::ProgressBar(static_cast<float>(m_current_step) / static_cast<float>(m_tutorial->getStepCount()), ImVec2(-1, 20));
  // ImGui::PopStyleColor();

  ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE));

  if (ImGui::Button("Back", ImVec2(100, 0)))
  {
    if (m_current_step != 0) {
      m_current_step--;
      std::cout << m_current_step << std::endl;
    }
  }
  
  ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - NEXT_BUTTON_SIZE_X);
  if (ImGui::Button("Next", ImVec2(-1, -1)))
  {
    if (m_current_step != m_tutorial->getStepCount() - 1) {
      m_current_step++;
      std::cout << m_current_step << std::endl;
    }
  }

  // POP STYLE
  ImGui::PopStyleColor();
  // END
  ImGui::EndChild();
}
