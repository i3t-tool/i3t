#include "TutorialWindow.h"
#include "Logger/Logger.h"
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
#include "Utils/TextureLoader.h"

#include <filesystem>

// TUTORIAL GUI PROPERTIES DEFINITIONS
//---

const int NEXT_BUTTON_SIZE_X = 150;
const int SIMPLE_SPACE = 10;
const int SMALL_SPACE = 5;
const int CONTROLS_SIZE_Y = 100;

// TEMPORARY TODO
static std::string current_dir = "";

// TEMPORARY TODO
inline ImGui::MarkdownImageData ImageCallback( ImGui::MarkdownLinkCallbackData data_ )
{
  // In your application you would load an image based on data_ input. Here we just use the imgui font texture.
  std::string image_path = current_dir + data_.link;
  image_path.pop_back(); // BUG FIX where there's an ")" in the data_.link member
  //std::cout << image_path.c_str() << std::endl;
  int tex_id = TextureLoader::loadTexture(data_.text, image_path);
  
  ImGui::MarkdownImageData imageData{ true, false, (ImTextureID)tex_id, ImVec2( 400.0f, 200.0f ) };

  // For image resize when available size.x > image width, add
  ImVec2 const contentSize = ImGui::GetContentRegionAvail();
  if( imageData.size.x > contentSize.x )
  {
    float const ratio = imageData.size.y/imageData.size.x;
    imageData.size.x = contentSize.x;
    imageData.size.y = contentSize.x*ratio;
  }

  return imageData;
}

// TUTORIAL WINDOW FUNCTIONS
//---

TutorialWindow::TutorialWindow(bool show) : IWindow(show)
{
  m_tutorial = nullptr;
  m_current_step = 0;
  m_mdConfig = ImGui::MarkdownConfig{nullptr, nullptr, ImageCallback, "link", { { nullptr, true }, { nullptr, true }, { nullptr, false } }, nullptr };

  // TEMPORARY todo
  TutorialHeader dummy_header;
  setTutorial(dummy_header);
}

void TutorialWindow::setTutorial(TutorialHeader header)
{
  m_tutorial = TutorialLoader::loadTutorial(std::move(header)); // btw if there was a previous unique pointer to another Tutorial, it gets deleted at this reassignment (yay, thats why we are using it! \^^/)
  setStep(0);

  std::cout << m_tutorial->m_header.m_filename;
  std::filesystem::path p(m_tutorial->m_header.m_filename);
  current_dir = p.parent_path().string() + "/";
  std::cout << current_dir;
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
  //ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(66, 150, 250, 255));
  //ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(66, 150, 250, 205));
  //ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(66, 150, 250, 102));
  //ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(245, 245, 245, 255));
  // BEGIN WINDOW
  const std::string window_name = "Tutorial - " + m_tutorial->m_header.m_title + "###Tutorial window";
  ImGui::Begin(window_name.c_str(), &Application::get().m_showTutorialWindow);

  // CREATE IMGUI CONTENT
  renderTutorialHeader();
  renderTutorialContent();
  renderTutorialControls();

  // POP STYLE
  ImGui::PopStyleVar(3);
  ImGui::PopStyleColor(6);
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

  if (title != "undefined") {
    ImGui::PushFont(App::get().getFont(FONT_TITLE));
    ImGui::TextWrapped(title.c_str());
    ImGui::PopFont();
    ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE)); // vertical spacing
  }
  else {
    ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE)); // vertical spacing
  }
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


  if (m_tutorial)
  {
    if (m_tutorial->getStepCount() > 0) {
      // ITERATE OVER WIDGETS IN CURRENT STEP AND RENDER THEM
      for (std::unique_ptr<TWidget>& widget_uptr : m_tutorial->m_steps[m_current_step].m_content) {
        widget_uptr->acceptRenderer(this);
      }
    }


  }
  else
  {
    // default content
    // todo eg "No tutorial loaded. You can close the window or choose one here:"
    // also change spacing to default imgui one

  }
  ImGui::EndChild();
}

void TutorialWindow::renderTextWidget(TWText* tw_text)
{
  ImGui::Markdown( tw_text->m_text.c_str(), tw_text->m_text.length(), m_mdConfig);
}

void TutorialWindow::renderImageWidget(TWImage* tw_image)
{
}

void TutorialWindow::renderAnimatedImageWidget(TWAnimatedImage* tw_animated_image)
{
}

void TutorialWindow::renderTaskWidget(TWTask* tw_task)
{
  ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE)); 
  ImGui::PushFont(App::get().getFont(FONT_TASK_TITLE));
  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(66, 150, 250, 255));

  ImGui::TextWrapped(tw_task->m_task.c_str());

  ImGui::PopStyleColor();
  ImGui::PopFont();
  ImGui::Dummy(ImVec2(0.0f, SMALL_SPACE));
}

void TutorialWindow::renderHintWidget(TWHint* tw_hint)
{
  //ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(66, 150, 250, 255));
  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
  std::string hint_header = u8"Nápovìda##" + std::to_string(m_current_step);
  if (ImGui::CollapsingHeader(hint_header.c_str())) {
    ImGui::PopStyleColor();
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(66, 150, 250, 255));
    //ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
    //ImGui::TextWrapped(tw_hint->m_hint.c_str());
    ImGui::Markdown( tw_hint->m_hint.c_str(), tw_hint->m_hint.length(), m_mdConfig);
    ImGui::PopStyleColor();
  }
  else {
    ImGui::PopStyleColor();
  }
}

void TutorialWindow::renderSpacingWidget(TWSpacing* tw_hint)
{
  ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE)); // vertical spacing
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
      //std::cout << m_current_step << std::endl;
    }
  }
  
  ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - NEXT_BUTTON_SIZE_X);
  if (ImGui::Button("Next", ImVec2(-1, 0)))
  {
    if (m_current_step != m_tutorial->getStepCount() - 1) {
      m_current_step++;
      //std::cout << m_current_step << std::endl;
    }
  }

  // POP STYLE
  ImGui::PopStyleColor();
  // END
  ImGui::EndChild();
}
