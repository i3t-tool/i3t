/**
 * \file
 * \brief
 * \authors Miroslav Müller, Adam Loucký
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "TutorialWindow.h"

#include <utility>

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include "GUI/UIModule.h"
#include "Logger/Logger.h"

/// \todo If you want to use ImGui Markdown run $ git submodule add
/// https://github.com/Mizumaky/imgui_markdown_extended
/// Depencencies/imgui_markdown. After that modify CMakeLists.txt, append
/// include_directories(Dependencies/imgui_markdown) to block of includes.
/// a taky zmenit v nem pridani do filtru ve VS a asi i dalsi veci - jeste se
/// zeptat!
#include <codecvt>

#include "imgui_markdown.h"

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "Core/Application.h"
#include "Tutorial/Tutorial.h"
#include "Tutorial/TutorialLoader.h"
#include "Utils/Other.h"

#include "API.h"
#include "StartWindow.h"
#include <filesystem>

#ifdef _WIN32
#include "GUI/Elements/Modals/BeforeNewTutModal.h"
#include "Shellapi.h"
#include <Windows.h>
#endif // _WIN32

#ifdef defined(unix) || defined(__unix__) || defined(__unix)
#define _UNIX
// todo
#endif // unix

// TUTORIAL GUI PROPERTIES DEFINITIONS
//---

const int NEXT_BUTTON_SIZE_X = 120;
const int NEXT_BUTTON_SIZE_Y = 30;
const int TIP_BUTTON_SIZE_X = 50;
const int TIP_BUTTON_SIZE_Y = 30;
const int SIMPLE_SPACE = 10;
const int SMALL_SPACE = 5;
const int CONTROLS_SIZE_Y = 90;
const int MIN_WIN_WIDTH = 500;
const int MIN_WIN_HEIGHT = 600;
// const int MAX_CONTENT_WIDTH = 600;

// TUTORIAL WINDOW FUNCTIONS
//---

// INIT
TutorialWindow::TutorialWindow(bool show) : IWindow(show)
{
	m_tutorial = nullptr;
	m_currentStep = 0;
	m_progressBarAnimationPosition = 0;
	SetTutorialCommand::addListener([this](std::shared_ptr<Tutorial> tutorial) {
		setTutorial(std::move(tutorial)); // COMMAND
	});
}

inline void TooltipCallback(ImGui::MarkdownTooltipCallbackData data_)
{
	ImGui::GetStyle().WindowRounding = I3T::getUI()->getTheme().get(ESize::Tooltip_Rounding);
	ImGui::GetStyle().WindowPadding = I3T::getUI()->getTheme().get(ESizeVec2::Tooltip_Padding);
	if (data_.linkData.isImage)
	{
		ImGui::SetTooltip("%.*s", data_.linkData.textLength, data_.linkData.text);
	}
	else
	{
		LOG_ERROR("Tooltip_CB - Markdown data doesnt contain image");
		// ImGui::SetTooltip( "%s Open in browser\n%.*s", data_.linkIcon,
		// data_.linkData.linkLength, data_.linkData.link );
	}
	ImGui::GetStyle().WindowRounding = I3T::getUI()->getTheme().get(ESize::Window_Rounding);
	ImGui::GetStyle().WindowPadding = I3T::getUI()->getTheme().get(ESizeVec2::Window_Padding);
}

inline void DefaultLinkCallback(ImGui::MarkdownLinkCallbackData data_)
{
	std::string url(data_.link, data_.linkLength);
	if (!data_.isImage)
	{
#ifdef _WIN32
		ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif // _WIN32
	}
}

void TutorialWindow::setTutorial(std::shared_ptr<Tutorial> tutorial)
{
	// MARKDOWN CONFIG - (temporarily?) moved here from constructor since
	// exception at font loading
	m_mdConfig = ImGui::MarkdownConfig{
	    DefaultLinkCallback,
	    TooltipCallback,
	    ImageCallback,
	    "link",
	    {{I3T::getUI()->getTheme().get(EFont::TutorialAssignment), true}, {nullptr, true}, {nullptr, false}},
	    nullptr};

	m_tutorial = std::move(tutorial); // btw if there was a previous shared pointer to
	                                  // another Tutorial, then if it isnt still used
	                                  // anywhere it gets deleted at this reassignment
	                                  // (yay, thats why we are using it! \^^/)
	m_progressBarAnimationPosition = 0;
	setStep(0);
}

void TutorialWindow::setTutorial(std::shared_ptr<TutorialHeader>& header)
{
	auto tutorial = TutorialLoader::loadTutorial(header);
	if (tutorial)
	{
		setTutorial(tutorial);
	}
	else
	{
		LOG_FATAL("Tutorial " + header->m_filename + " not loaded.");
	}
}

void TutorialWindow::setTutorial(std::string path)
{
	auto header = TutorialLoader::loadTutorialHeader(path);
	if (header)
	{
		setTutorial(header);
	}
	else
	{
		LOG_FATAL("Tutorial header " + path + " not loaded.");
	}
}

void TutorialWindow::setStep(int step_number)
{
	if (m_tutorial == nullptr || step_number < 0 || step_number >= m_tutorial->getStepCount())
	{
		LOG_FATAL("Trying to set an invalid step number or tutorial not active");
	}
	else
	{
		// set step
		m_currentStep = step_number;
		// run script
		if (!m_tutorial->m_steps[m_currentStep].m_scriptToRunWhenShown.empty())
		{
			ConsoleCommand::dispatch(m_tutorial->m_steps[m_currentStep].m_scriptToRunWhenShown);
		}
	}
}

//---

void TutorialWindow::render()
{
	if (!isVisible())
		return; // this should not be needed, but it saves pushing and popping the
		        // styles

	// PUSH STYLE
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, I3T::getUI()->getTheme().get(ESizeVec2::TutorialWindow_Padding));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, I3T::getUI()->getTheme().get(ESize::TutorialWindow_FrameRounding));
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, I3T::getUI()->getTheme().get(ESize::TutorialWindow_ScrollbarSize));
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding,
	                    I3T::getUI()->getTheme().get(ESize::TutorialWindow_ScrollbarRounding));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(MIN_WIN_WIDTH, MIN_WIN_HEIGHT));
	ImGui::PushStyleColor(ImGuiCol_TabActive, I3T::getUI()->getTheme().get(EColor::DockTabActive));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, I3T::getUI()->getTheme().get(EColor::TutorialBgColor));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, I3T::getUI()->getTheme().get(EColor::TutorialBgColor));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, I3T::getUI()->getTheme().get(EColor::TutorialScrollbarBg));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, I3T::getUI()->getTheme().get(EColor::TutorialScrollbarGrab));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered,
	                      I3T::getUI()->getTheme().get(EColor::TutorialScrollbarHovered));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, I3T::getUI()->getTheme().get(EColor::TutorialScrollbarActive));
	//  BEGIN WINDOW
	std::string window_name;
	if (m_tutorial != nullptr)
	{
		// window_name = "Tutorial - " + m_tutorial->m_header->m_title +
		// "###Tutorial window";
		window_name = setName("Tutorial");
	}
	else
	{
		// window_name = "Tutorial - empty###Tutorial window";
		window_name = setName("Tutorial");
	}

	// START THE WINDOW

	ImGui::SetNextWindowSize(ImVec2(500, 650), ImGuiCond_FirstUseEver);
	ImGui::Begin(window_name.c_str(), getShowPtr());

	this->updateWindowInfo();

	ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialText));

	// CREATE IMGUI CONTENT
	renderTutorialHeader();
	renderTutorialContent();
	renderTutorialControls();

	// POP STYLE
	ImGui::PopStyleVar(5);
	ImGui::PopStyleColor(7);
	ImGui::PopStyleColor();
	// END WINDOW
	ImGui::End();
}

void TutorialWindow::renderTutorialHeader()
{
	std::string title;
	if (m_tutorial == nullptr)
	{
		title = "No tutorial";
	}
	else if (m_tutorial->getStepCount() > 0)
	{
		title = m_tutorial->m_header->m_title;
	}
	else
	{
		title = "Empty tutorial";
	}

	// display title if not "undefined"
	if (title != "undefined")
	{
		ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialTitleText));
		ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::TutorialTitle));
		ImGui::TextWrapped(title.c_str());
		ImGui::PopFont();
		ImGui::PopStyleColor();
	}
	// additional vertical spacing after title
	ImGui::Dummy(ImVec2(0.0f, SMALL_SPACE));
	// ImGui::EndChild();
}

void TutorialWindow::renderTutorialContent()
{
	// PUSH STYLE
	// ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	// BEGIN CHILD
	ImGui::BeginChild("content",
	                  ImVec2(0,
	                         -(CONTROLS_SIZE_Y + ImGui::GetStyle().ItemSpacing.y))); // be of size: [remaining Y space]
	                                                                                 // - [planned Y size of contents]
	// btw ImGui::GetContentRegionAvail().y - remaining space Y
	// ImGui::PopStyleVar();

	// SET UP MARKDOWN
	//    ImGuiIO& io = ImGui::GetIO();
	// io.Fonts->Clear();
	//// Base font
	// io.Fonts->AddFontFromFileTTF( "myfont.ttf", fontSize_ );
	//// Bold headings H2 and H3
	// H2 = io.Fonts->AddFontFromFileTTF( "myfont-bold.ttf", fontSize_ );
	// H3 = mdConfig.headingFormats[ 1 ].font;
	//// bold heading H1
	// float fontSizeH1 = fontSize_ * 1.1f;
	// H1 = io.Fonts->AddFontFromFileTTF( "myfont-bold.ttf", fontSizeH1 );

	if (m_tutorial)
	{
		// RENDER TUTORIAL
		if (m_tutorial->getStepCount() > 0)
		{
			// ITERATE OVER WIDGETS IN CURRENT STEP AND RENDER THEM
			for (std::shared_ptr<TutorialElement>& widget_uptr : m_tutorial->m_steps[m_currentStep].m_content)
			{
				widget_uptr->acceptRenderer(this);
			}
		}
	}
	else
	{
		// default content
		ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::TutorialTitle));
		ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialButtonText));
		ImGui::PushStyleColor(ImGuiCol_Button, I3T::getUI()->getTheme().get(EColor::TutorialButtonBg));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getUI()->getTheme().get(EColor::TutorialButtonActive));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getUI()->getTheme().get(EColor::TutorialButtonHovered));
		if (ImGui::Button("Open Start Menu", ImVec2(-1, NEXT_BUTTON_SIZE_Y * 5)))
		{
			*I3T::getWindowPtr<StartWindow>()->getShowPtr() = true;
			this->hide();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}
		ImGui::PopStyleColor(4);
		ImGui::PopFont();
	}
	ImGui::EndChild();
}

void TutorialWindow::renderTutorialControls()
{
	if (m_tutorial)
	{
		// PUSH STYLE
		ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialText));
		// BEGIN CHILD
		ImGui::BeginChild("controls", ImVec2(0, 0)); // stretch remaining Y space
		ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE));    // vertical spacing

		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, I3T::getUI()->getTheme().get(EColor::TutorialButtonBg));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, I3T::getUI()->getTheme().get(EColor::TutorialBarBg));

		const int stepCount = m_tutorial->getStepCount();
		std::string progressString =
		    std::to_string(m_currentStep + 1) + "/" + std::to_string(m_tutorial->getStepCount());
		ImGui::ProgressBar(m_progressBarAnimationPosition, ImVec2(-1, 20), progressString.c_str());
		// progress animation
		float animationDiff =
		    m_progressBarAnimationPosition - static_cast<float>(m_currentStep + 1) / static_cast<float>(stepCount);
		if (abs(animationDiff) < 0.003)
		{
			// snap to position
			m_progressBarAnimationPosition = static_cast<float>(m_currentStep + 1) / static_cast<float>(stepCount);
		}
		else
		{
			// approach desired position
			m_progressBarAnimationPosition -= animationDiff / 4;
		}

		ImGui::PopStyleColor(2);

		ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE));

		// BUTTONS
		ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::Button));
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
		ImGui::PushStyleColor(ImGuiCol_Button, I3T::getUI()->getTheme().get(EColor::TutorialBgColor));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getUI()->getTheme().get(EColor::TutorialBgColor));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getUI()->getTheme().get(EColor::TutorialBgColor));
		// Back button
		if (m_currentStep != 0)
		{
			if (ImGui::Button("< Back", ImVec2(I3T::getUI()->getTheme().get(ESize::TutorialWindow_BackButtonWidth),
			                                   NEXT_BUTTON_SIZE_Y)))
			{
				setStep(m_currentStep - 1);
				// std::cout << m_currentStep << std::endl;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}
		}
		else
		{
			if (ImGui::Button("< Start Menu",
			                  ImVec2(I3T::getUI()->getTheme().get(ESize::TutorialWindow_MainMenuButtonWidth),
			                         NEXT_BUTTON_SIZE_Y)))
			{
				*I3T::getWindowPtr<StartWindow>()->getShowPtr() = true;
				this->hide();
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}
		}
		ImGui::PopStyleColor(4);

		// spacing
		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - NEXT_BUTTON_SIZE_X);

		// Next button
		ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialButtonText));
		ImGui::PushStyleColor(ImGuiCol_Button, I3T::getUI()->getTheme().get(EColor::TutorialButtonBg));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getUI()->getTheme().get(EColor::TutorialButtonActive));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getUI()->getTheme().get(EColor::TutorialButtonHovered));
		if (m_currentStep < m_tutorial->getStepCount() - 1)
		{
			if (ImGui::Button("Next", ImVec2(-1, NEXT_BUTTON_SIZE_Y)))
			{
				setStep(m_currentStep + 1);
				// std::cout << m_currentStep << std::endl;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}
		}
		else
		{
			if (ImGui::Button("Finish", ImVec2(-1, NEXT_BUTTON_SIZE_Y)))
			{
				m_tutorial = nullptr;
				*I3T::getWindowPtr<StartWindow>()->getShowPtr() = true;
				this->hide();
				// std::cout << m_currentStep << std::endl;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(4);

		// END CHILD
		ImGui::EndChild();
		// POP STYLE
		ImGui::PopStyleColor();
	}
}

// TUTORIAL ELEMENTS
void TutorialWindow::renderExplanation(Explanation* explanation)
{
	ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE));
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, I3T::getUI()->getTheme().get(EColor::TutorialHighlightText));

	ImGui::PushStyleColor(ImGuiCol_PopupBg, I3T::getUI()->getTheme().get(EColor::TutorialBgColor));
	ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::TutorialText));
	ImGui::Markdown(explanation->m_content.c_str(), explanation->m_content.length(), m_mdConfig);
	ImGui::PopStyleColor(2);
	ImGui::PopFont();
}

void TutorialWindow::renderHeadline(Headline* headline)
{
	ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE));
	ImGui::SetWindowFontScale(1.1f);
	ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::TutorialAssignment));
	ImGui::Markdown(headline->m_content.c_str(), headline->m_content.length(), m_mdConfig);
	ImGui::SetWindowFontScale(1.f);
	ImGui::PopFont();
}

void TutorialWindow::renderChoiceTask(ChoiceTask* choice) {}

void TutorialWindow::renderMultiChoiceTask(MultiChoiceTask* multiChoice) {}

void TutorialWindow::renderInputTask(InputTask* input) {}

void TutorialWindow::renderTask(Task* task)
{
	// Calculate font size multiplier relative to default imgui fontsize (10) minus space between paragraphs
	float fontMult = (ImGui::GetFontSize() - SIMPLE_SPACE / 4) / 10;
	// Rounding to 1 decimal for consistency
	fontMult = static_cast<float>(static_cast<int>(fontMult * 10.)) / 10.;

	float squareSize = ImGui::GetFontSize() - ImGui::GetStyle().FramePadding.y * fontMult; // zde velikost potrebujeme
	// Task background rendering
	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	float winXMin = ImGui::GetWindowDrawList()->GetClipRectMin().x;
	float winXMax = ImGui::GetWindowDrawList()->GetClipRectMax().x;
	// Send the window width for calculating wrapping (padding on both sides and simple spaces to account for square
	float wrapWidth = winXMax - winXMin - squareSize - ImGui::GetStyle().WindowPadding.x * 2;

	// Find the amount of rows the task has
	float bgY = ImGui::CalcTextSize(task->m_content.c_str(), NULL, false, wrapWidth).y * fontMult;

	// Draw square, accounting for the task offset
	ImVec2 p_min = ImVec2(cursorPos.x, cursorPos.y + SIMPLE_SPACE);
	ImVec2 p_max = ImVec2(winXMax, cursorPos.y + SIMPLE_SPACE + SIMPLE_SPACE / 2 + bgY);
	ImU32 bgColor = ImGui::ColorConvertFloat4ToU32(I3T::getUI()->getTheme().get(EColor::TutorialTaskBg));
	ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, bgColor);
	ImGui::SetCursorScreenPos(cursorPos);

	// Space to offset text
	ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE));

	// Draw task blue square
	ImVec2 drawPos =
	    ImGui::GetCursorScreenPos() +
	    ImVec2(I3T::getUI()->getTheme().get(ESize::TutorialTaskSquareXPadding), ImGui::GetStyle().FramePadding.y);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImU32 color = ImGui::ColorConvertFloat4ToU32(I3T::getUI()->getTheme().get(EColor::TutorialTitleText));
	draw_list->AddRectFilled(ImVec2(drawPos.x, drawPos.y), ImVec2(drawPos.x + squareSize, drawPos.y + squareSize),
	                         color);
	ImGui::Dummy(ImVec2(squareSize + I3T::getUI()->getTheme().get(ESize::TutorialTaskSquareXPadding), squareSize));
	ImGui::SameLine();

	// Load task font and color
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, I3T::getUI()->getTheme().get(EColor::TutorialHighlightText));
	ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::TutorialAssignment));

	// Print text from markdown
	ImGui::Markdown(task->m_content.c_str(), task->m_content.length(), m_mdConfig);

	ImGui::PopStyleColor();
	ImGui::PopFont(); // velikost zpet
}

void TutorialWindow::renderHint(Hint* hint)
{
	ImGui::Dummy(ImVec2(0.0f, SMALL_SPACE));

	ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::TutorialAssignment));
	ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialButtonText));
	if (!hint->m_expanded)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, I3T::getUI()->getTheme().get(EColor::TutorialButtonBg));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getUI()->getTheme().get(EColor::TutorialButtonHovered));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getUI()->getTheme().get(EColor::TutorialButtonActive));
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, I3T::getUI()->getTheme().get(EColor::TutorialButtonBg));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getUI()->getTheme().get(EColor::TutorialButtonHovered));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getUI()->getTheme().get(EColor::TutorialButtonActive));
	}
	// Hint button
	if (ImGui::Button("Tip", ImVec2(TIP_BUTTON_SIZE_X, TIP_BUTTON_SIZE_Y)))
	{
		hint->m_expanded = !hint->m_expanded;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}
	ImGui::PopStyleColor(4);
	ImGui::PopFont();

	// Hint itself
	if (hint->m_expanded)
	{
		ImGui::PushStyleColor(ImGuiCol_PopupBg, I3T::getUI()->getTheme().get(EColor::TutorialBgColor));
		ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialTitleText));
		ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::TutorialHint));
		ImGui::Indent();
		ImGui::Markdown(hint->m_content.c_str(), hint->m_content.length(), m_mdConfig);
		ImGui::Unindent();
		ImGui::PopFont();
		ImGui::PopStyleColor(2);
	}
}

// TODO (later when moving from imgui markdown to a custom solution) - make it
// not static again
std::shared_ptr<Tutorial> TutorialWindow::m_tutorial;
inline ImGui::MarkdownImageData TutorialWindow::ImageCallback(ImGui::MarkdownLinkCallbackData data_)
{
	std::string imageFilename(data_.link, data_.linkLength);

	// try to find the texture, if it isnt loaded, then load it
	std::shared_ptr<GUIImage> img = nullptr;

	if (m_tutorial != nullptr)
	{
		// image already loaded
		if (m_tutorial->m_filenameToImage.contains(imageFilename))
		{
			img = m_tutorial->m_filenameToImage[imageFilename];
		}
		// image not yet loaded
		else
		{
			LOG_INFO("Loading image " + imageFilename);
			img = TutorialLoader::loadImage(TutorialLoader::getDirectory(m_tutorial->m_header->m_filename) +
			                                imageFilename);
			m_tutorial->m_filenameToImage[imageFilename] = img; // save for future use
		}
	}
	else
	{
		LOG_FATAL("Tutorial is nullptr");
	}

	ImGui::MarkdownImageData imageData;
	// nullptr check (todo possibly use a dummy image)
	if (img != nullptr)
	{
		imageData = {true, false, (ImTextureID) img->m_texID, ImVec2(img->m_width, img->m_height)};
	}
	else
	{
		imageData = {false, false, nullptr, ImVec2(10.0f, 10.0f)};
	}

	// For image resize when available size.x > image width, add
	ImVec2 const contentSize = ImGui::GetContentRegionAvail();
	if (imageData.size.x > contentSize.x)
	{
		float const ratio = imageData.size.y / imageData.size.x;
		imageData.size.x = contentSize.x;
		imageData.size.y = contentSize.x * ratio;
	}

	return imageData;
}
