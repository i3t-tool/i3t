#include "TutorialWindow.h"

#include <utility>

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include "GUI/UIModule.h"
#include "Logger/Logger.h"

/// \todo If you want to use ImGui Markdown run $ git submodule add https://github.com/Mizumaky/imgui_markdown_extended
/// Depencencies/imgui_markdown. After that modify CMakeLists.txt, append
/// include_directories(Dependencies/imgui_markdown) to block of includes.
/// a taky zmenit v nem pridani do filtru ve VS a asi i dalsi veci - jeste se zeptat!
#include "GUI/ImGui/imgui_markdown.h"

#include "Config.h"
#include "Commands/ApplicationCommands.h"
#include "Core/Application.h"
#include "Tutorial/Tutorial.h"
#include "Tutorial/TutorialLoader.h"
#include "Utils/Other.h"
#include "Utils/TextureLoader.h"

#include <filesystem>
#include <Core/API.h>
#include "IntroWindow.h"


// TUTORIAL GUI PROPERTIES DEFINITIONS
//---

const int NEXT_BUTTON_SIZE_X = 120;
const int SIMPLE_SPACE = 10;
const int SMALL_SPACE = 5;
const int CONTROLS_SIZE_Y = 100;

// TEMPORARY TODO
std::shared_ptr<Tutorial> TutorialWindow::m_tutorial;
inline ImGui::MarkdownImageData TutorialWindow::ImageCallback(ImGui::MarkdownLinkCallbackData data_)
{


  // In your application you would load an image based on data_ input. Here we just use the imgui font texture.
  std::string imageFilename = data_.link;
  imageFilename.pop_back(); // VERY TEMPORARY IMGUI_MD BUG FIX where there's an "\n" in the data_.link member
  imageFilename.pop_back(); // BUG FIX where there's an ")" in the data_.link member
  //std::cout << image_path.c_str() << std::endl;

  // try to find the texture, if it isnt loaded, then load it
  int tex_id = 0;
  // todo temporary
  if (m_tutorial != nullptr) {
    if ( m_tutorial->m_filenameToImage.contains(imageFilename)) {
      auto img = m_tutorial->m_filenameToImage[imageFilename];
      if (img != nullptr) {
        tex_id = img->m_texID;
      }
      else {
        //todo use dummy image
        //Log::info("Using dummy image");
      }
    }
    else {
      Log::fatal("Image " + imageFilename + " not loaded by loader");
    }
  }
  else {
    Log::fatal("Tutorial is nullptr");
  }



  //else if (const auto it{ m_tutorial->m_filenameToImage.find(image_path) }; it != std::end(m_tutorial->m_filenameToImage)) {
  //  tex_id = it->second->m_texID;
  //}
  //else {
  //  try {
  //    m_tutorial->m_filenameToImage[image_path] = std::make_shared<GUIImage>(image_path);
  //    tex_id = m_tutorial->m_filenameToImage[image_path]->m_texID;
  //  }
  //  catch (const std::runtime_error& error) {
  //    tex_id = 0;
  //  }
  //}
  
  //int tex_id = TextureLoader::loadTexture(data_.text, image_path);
  
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

  SetTutorialCommand::addListener([this](std::shared_ptr<Tutorial> tutorial) {
	  setTutorial(std::move(tutorial));
  });

  //// TEMPORARY todo
  //std::shared_ptr<TutorialHeader> dummy_header = std::make_shared<TutorialHeader>("none");
  //setTutorial(dummy_header);
}

void TutorialWindow::setTutorial(std::shared_ptr<Tutorial> tutorial)
{
  m_tutorial = std::move(tutorial);
	// btw if there was a previous shared pointer to another Tutorial, then if it isnt still used anywhere it gets deleted at this reassignment (yay, thats why we are using it! \^^/)
	m_current_step = 0;

	// todo make a utility function for this
  //std::cout << m_tutorial->m_header->m_filename;
  //std::filesystem::path p(m_tutorial->m_header->m_filename);
  //m_current_dir = p.parent_path().string() + "/";
  //std::cout << m_current_dir;
}

void TutorialWindow::setTutorial(std::shared_ptr<TutorialHeader>& header)
{
  auto tutorial = TutorialLoader::loadTutorial(header);
  if (tutorial) {
	  setTutorial(tutorial);
  }
	else {
    Log::fatal("Tutorial " + header->m_filename + " not loaded.");
	}
}

void TutorialWindow::setTutorial(std::string path)
{
  auto header = TutorialLoader::loadTutorialHeader(path);
  if (header) {
    setTutorial(header);
  }
  else {
    Log::fatal("Tutorial header " + path + " not loaded." );
  }
}

void TutorialWindow::setStep(int step_number)
{
  if (m_tutorial == nullptr || step_number < 0 || step_number >= m_tutorial->getStepCount())
  {
  	Log::fatal("Trying to set an invalid step number or tutorial not active");
  }
	else {
	  m_current_step = step_number;
	}
}

//---

void TutorialWindow::render()
{
  if (!isVisible())
    return;

  // PUSH STYLE 
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30.0f, 35.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
  ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 20);
  ImGui::PushStyleColor(ImGuiCol_WindowBg, Application::get().getUI()->getTheme().get(EColor::TutorialBgColor));
  ImGui::PushStyleColor(ImGuiCol_ChildBg, Application::get().getUI()->getTheme().get(EColor::TutorialBgColor));
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
  std::string window_name;
  if (m_tutorial != nullptr) {
    window_name = "Tutorial - " + m_tutorial->m_header->m_title + "###Tutorial window";
  }
  else {
    window_name = "Tutorial - empty###Tutorial window";
  }
  ImGui::SetNextWindowSize(ImVec2(400,300));
  ImGui::Begin(window_name.c_str(), getShowPtr());

  // CREATE IMGUI CONTENT
  renderTutorialHeader();
  renderTutorialContent();
  renderTutorialControls();

  // POP STYLE
  ImGui::PopStyleVar(3);
  ImGui::PopStyleColor(7);
  // END WINDOW
  ImGui::End();
}

void TutorialWindow::renderTutorialHeader()
{
  //ImGui::BeginChild("header");
  std::string title;
  if (m_tutorial == nullptr){
    title = "No tutorial";
  }
  else if (m_tutorial->getStepCount() > 0) {
    title = m_tutorial->m_header->m_title;
  }
  else {
    title = "Empty tutorial";
  }

  // display title if not "undefined"
  if (title != "undefined") {
    ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::Title));
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
      for (std::shared_ptr<TutorialElement>& widget_uptr : m_tutorial->m_steps[m_current_step].m_content) {
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

void TutorialWindow::renderTutorialControls()
{
	if (m_tutorial) {
		// PUSH STYLE
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
		// BEGIN CHILD
		ImGui::BeginChild("controls", ImVec2(0,0)); // stretch remaining Y space
		ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE));   // vertical spacing

		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(14, 98, 175, 255));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(215, 215, 215, 255));
		const int   progress       = m_tutorial->getStepCount();
		std::string progressString = std::to_string(m_current_step + 1) + "/" + std::to_string(m_tutorial->getStepCount());
		ImGui::ProgressBar(static_cast<float>(m_current_step + 1) / static_cast<float>(progress), ImVec2(-1, 20), progressString.c_str());
		ImGui::PopStyleColor(2);

		ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE));

		// BUTTONS
		ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::Button));
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(14, 98, 175, 255));
		ImGui::PushStyleColor(ImGuiCol_Button, Application::get().getUI()->getTheme().get(EColor::TutorialBgColor));
		if (m_current_step != 0) {
			if (ImGui::Button("< Back", ImVec2(50, 0)))
			{
				m_current_step--;
				//std::cout << m_current_step << std::endl;
			}
		}
		else {
			ImGui::InvisibleButton("< Back", ImVec2(10, 10));
		}
		ImGui::PopStyleColor(2);
		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - NEXT_BUTTON_SIZE_X);
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(14, 98, 175, 255));
		if (m_current_step != m_tutorial->getStepCount() - 1) {
			if (ImGui::Button("Next", ImVec2(-1, 0)))
			{
				m_current_step++;
				//std::cout << m_current_step << std::endl;
			}
		}
		else {
			if (ImGui::Button("Finish", ImVec2(-1, 0)))
			{
				m_tutorial = nullptr;
				*I3T::getWindowPtr<IntroWindow>()->getShowPtr() = true;
				//std::cout << m_current_step << std::endl;
			}
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(2);
	
		// END CHILD
		ImGui::EndChild();
		// POP STYLE
		ImGui::PopStyleColor();
	}

}

// TUTORIAL ELEMENTS
void TutorialWindow::renderExplanation(Explanation* explanation)
{
  ImGui::Markdown( explanation->m_content.c_str(), explanation->m_content.length(), m_mdConfig);
}

void TutorialWindow::renderChoiceTask(ChoiceTask* choice)
{
}

void TutorialWindow::renderMultiChoiceTask(MultiChoiceTask* multiChoice)
{
}

void TutorialWindow::renderInputTask(InputTask* input)
{
}

void TutorialWindow::renderTask(Task* task)
{
  ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE)); 
  ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::TaskTitle));
  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(14, 98, 175, 255));

	//ImVec2 cursorPos = ImGui::GetCursorScreenPos();
 // float bgSizeY = ImGui::CalcTextSize(task->m_content.c_str()).y;
	//float winXMin = ImGui::GetWindowDrawList()->GetClipRectMin().x;
	//float winXMax = ImGui::GetWindowDrawList()->GetClipRectMax().x;
 // ImVec2 p_min = ImVec2(winXMin, cursorPos.y);
	//ImVec2 p_max = ImVec2(winXMax, cursorPos.y + bgSizeY);	
  //ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, IM_COL32(245, 249, 252, 255));
  //ImGui::SetCursorScreenPos(cursorPos);
  ImGui::Markdown(task->m_content.c_str(), task->m_content.length(), m_mdConfig);

  ImGui::PopStyleColor();
  ImGui::PopFont();
  ImGui::Dummy(ImVec2(0.0f, SMALL_SPACE));
}

void TutorialWindow::renderHint(Hint* hint)
{
	//ImGui::PushStyleVar(ImGuiStyleVar_);

  //ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(14, 98, 175, 255));
  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
  //std::u8string hintHeaderU8 = u8"Nápověda##";
  //std::string hintHeader(hintHeaderU8.cbegin(), hintHeaderU8.cend()); // todo find better solution
	std::string hintHeader = "Hint##";
  hintHeader += std::to_string(m_current_step);
	static bool toggled = false;
	static int lastStep = m_current_step;
	if (lastStep != m_current_step) {
		lastStep = m_current_step;
		toggled = false;
	}
	if (ImGui::SmallButton(hintHeader.c_str())) {
		toggled = !toggled;
	}
  if (toggled) {
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(14, 98, 175, 255));
    //ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
    //ImGui::TextWrapped(tw_hint->m_hint.c_str());
  	//ImGui::Indent();
    ImGui::Markdown( hint->m_content.c_str(), hint->m_content.length(), m_mdConfig);
  	//ImGui::Unindent();
  	
    ImGui::PopStyleColor();
  }
  ImGui::PopStyleColor();
	//if (ImGui::IsItemActive())
	//{
	//	ImGui::Text("open");
	//}
	//else {
	//	ImGui::Text("closed");
	//}
}

