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
#include "GUI/Elements/Modals/BeforeNewTutModal.h"
#include "GUI/Toolkit.h"
#include "Tutorial/Tutorial.h"
#include "Tutorial/TutorialLoader.h"
#include "Tutorial/TutorialManager.h"
#include "Utils/Other.h"

#include "I3T.h"
#include "StartWindow.h"
#include <filesystem>

#ifdef _WIN32
#include "Shellapi.h"
#include <Windows.h>
#endif // _WIN32

#ifdef defined(unix) || defined(__unix__) || defined(__unix)
#define _UNIX
// todo
#endif // unix

#include "GUI/Fonts/Icons.h"
#include "GUI/I3TGui.h"
#include "Localization/Localization.h"
#include "Scripting/Environment.h"

// TUTORIAL GUI PROPERTIES DEFINITIONS
//---

// TODO: (DR) (DPI-FIX) Cannot be constant
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
TutorialWindow::TutorialWindow(bool show) : IWindow(_t("Tutorial"), show)
{
	emptyTutorial();
	SetTutorialCommand::addListener([this](std::shared_ptr<Tutorial> tutorial) {
		setTutorial(std::move(tutorial)); // COMMAND
	});
	SetTutorialStepCommand::addListener([](int step) {
		TutorialManager::instance().setStep(step); // COMMAND
	});
}

bool TutorialWindow::hasTutorial() const
{
	return TutorialManager::instance().getTutorial() != nullptr;
}

void TutorialWindow::emptyTutorial()
{
	TutorialManager::instance().setTutorial(nullptr);
	m_progressBarAnimationPosition = 0;
}

inline void TooltipCallback(ImGui::MarkdownTooltipCallbackData data_)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, I3T::getUI()->getTheme().get(ESize::Tooltip_Rounding));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, I3T::getUI()->getTheme().get(ESizeVec2::Tooltip_Padding));
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
	ImGui::PopStyleVar(2);
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

void I3TMarkdownFormatCallback(const ImGui::MarkdownFormatInfo& markdownFormatInfo_, bool start_)
{
	// Call the default first so any settings can be overwritten by our implementation.
	// Alternatively could be called or not called in a switch statement on a case by case basis.
	// See defaultMarkdownFormatCallback definition for furhter examples of how to use it.
	ImGui::defaultMarkdownFormatCallback(markdownFormatInfo_, start_);

	switch (markdownFormatInfo_.type)
	{
	case ImGui::MarkdownFormatType::EMPHASIS:
	{
		if (markdownFormatInfo_.level == 1)
		{
			// in I3T for some reason, we force normal (italicized) emphasis to be strong emphasis (bold text)
			if (start_)
			{
				ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::TutorialBold));
			}
			else
			{
				ImGui::PopFont();
			}
		}
		break;
	}
	case ImGui::MarkdownFormatType::HEADING:
	{
		// Increase H1 font size
		if (markdownFormatInfo_.level == 1)
		{
			if (start_)
			{
				ImGui::SetWindowFontScale(1.14f);
			}
			else
			{
				ImGui::SetWindowFontScale(1.0f);
			}
		}
		break;
	}
	default:
		break;
	}
}

ImGui::MarkdownConfig TutorialWindow::createMarkdownConfig()
{
	ImFont* H1 = I3T::getUI()->getTheme().get(EFont::TutorialBold);
	ImFont* H2 = H1;
	ImFont* H3 = H2;
	return ImGui::MarkdownConfig{DefaultLinkCallback,
	                             TooltipCallback,
	                             ImageCallback,
	                             "link",
	                             {{H1, true}, {H2, true}, {H3, false}},
	                             nullptr,
	                             I3TMarkdownFormatCallback};
}

void TutorialWindow::setTutorial(std::shared_ptr<Tutorial> tutorial)
{
	// MARKDOWN CONFIG - (temporarily?) moved here from constructor since
	// exception at font loading
	m_mdConfig = createMarkdownConfig();

	TutorialManager::instance().setTutorial(std::move(tutorial));

	m_progressBarAnimationPosition = 0;
}

void TutorialWindow::setTutorial(std::shared_ptr<TutorialHeader>& header)
{
	auto tutorial = TutorialLoader::loadTutorial(header);
	if (!tutorial)
	{
		LOG_FATAL("Tutorial {} not loaded.", header->m_filename.string());
		return;
	}

	SetTutorialCommand::dispatch(tutorial);
}

void TutorialWindow::reloadTutorial()
{
	if (hasTutorial())
	{
		setTutorial(TutorialManager::instance().getTutorial()->m_header);
	}
}

Ptr<Tutorial> TutorialWindow::getTutorial()
{
	return TutorialManager::instance().getTutorial();
}

//---

void TutorialWindow::render()
{
	if (!isVisible())
		return; // this should not be needed, but it saves pushing and popping the
		        // styles

	// PUSH STYLE
	GUI::dockTabStylePush();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, I3T::getUI()->getTheme().get(ESizeVec2::Tutorial_WindowPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, I3T::getUI()->getTheme().get(ESize::Tutorial_FrameRounding));
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, I3T::getUI()->getTheme().get(ESize::Tutorial_ScrollbarSize));
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding,
	                    I3T::getUI()->getTheme().get(ESize::Tutorial_ScrollbarRounding));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(MIN_WIN_WIDTH, MIN_WIN_HEIGHT));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, I3T::getUI()->getTheme().get(EColor::TutorialBgColor));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, I3T::getUI()->getTheme().get(EColor::TutorialBgColor));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, I3T::getUI()->getTheme().get(EColor::TutorialScrollbarBg));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, I3T::getUI()->getTheme().get(EColor::TutorialScrollbarGrab));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered,
	                      I3T::getUI()->getTheme().get(EColor::TutorialScrollbarHovered));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, I3T::getUI()->getTheme().get(EColor::TutorialScrollbarActive));

	//  BEGIN WINDOW
	auto tutorial = TutorialManager::instance().getTutorial();
	if (tutorial != nullptr)
	{
		// window_name = "Tutorial - " + tutorial->m_header->m_title +
		// "###Tutorial window";
		setTitle(std::string("Tutorial - ") + tutorial->m_header->m_title);
	}
	else
	{
		// window_name = "Tutorial - empty###Tutorial window";
		setTitle("Tutorial");
	}

	// START THE WINDOW

	ImGui::SetNextWindowSize(ImVec2(500, 650), ImGuiCond_FirstUseEver);
	ImGui::Begin(getName(), getShowPtr());
	this->updateWindowInfo();
	GUI::dockTabStylePop();

	ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialText));

	// CREATE IMGUI CONTENT
	renderTutorialHeader();
	renderTutorialContent();
	renderTutorialControls();

	// POP STYLE
	ImGui::PopStyleVar(5);
	ImGui::PopStyleColor(7);
	// END WINDOW
	ImGui::End();
}

void TutorialWindow::renderTutorialHeader()
{
	auto tutorial = TutorialManager::instance().getTutorial();
	std::string title;

	if (tutorial == nullptr)
	{
		title = _t("No tutorial");
	}
	else if (tutorial->getStepCount() > 0)
	{
		title = tutorial->m_header->m_title;
	}
	else
	{
		title = _t("Empty tutorial");
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

	auto tutorial = TutorialManager::instance().getTutorial();

	if (tutorial)
	{
		// RENDER TUTORIAL
		if (tutorial->getStepCount() > 0)
		{
			auto& currentStep = tutorial->m_steps[TutorialManager::instance().getStep()];
			// ITERATE OVER WIDGETS IN CURRENT STEP AND RENDER THEM
			for (std::shared_ptr<TutorialElement>& widget_uptr : currentStep.m_content)
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
		if (I3TGui::ButtonWithLog(_t("Open Start Menu"), ImVec2(-1, NEXT_BUTTON_SIZE_Y * 5)))
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
	auto tutorial = TutorialManager::instance().getTutorial();

	if (!tutorial)
	{
		return;
	}

	// PUSH STYLE
	ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialText));
	// BEGIN CHILD
	ImGui::BeginChild("controls", ImVec2(0, 0)); // stretch remaining Y space
	ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE));    // vertical spacing

	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, I3T::getUI()->getTheme().get(EColor::TutorialButtonBg));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, I3T::getUI()->getTheme().get(EColor::TutorialBarBg));

	const int stepCount = tutorial->getStepCount();
	const int currentStep = TutorialManager::instance().getStep();

	std::string progressString = std::to_string(currentStep + 1) + "/" + std::to_string(tutorial->getStepCount());
	ImGui::ProgressBar(m_progressBarAnimationPosition, ImVec2(-1, 20), progressString.c_str());
	// progress animation
	float animationDiff =
	    m_progressBarAnimationPosition - static_cast<float>(currentStep + 1) / static_cast<float>(stepCount);
	if (abs(animationDiff) < 0.003)
	{
		// snap to position
		m_progressBarAnimationPosition = static_cast<float>(currentStep + 1) / static_cast<float>(stepCount);
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
	if (currentStep != 0)
	{
		if (I3TGui::ButtonWithLog(_t("< Back"), ImVec2(I3T::getUI()->getTheme().get(ESize::Tutorial_BackButtonWidth),
		                                               NEXT_BUTTON_SIZE_Y)))
		{
			TutorialManager::instance().setStep(currentStep - 1);
			// std::cout << m_currentStep << std::endl;
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}
	}
	else
	{
		if (I3TGui::ButtonWithLog(
		        _t("< Start Menu"),
		        ImVec2(I3T::getUI()->getTheme().get(ESize::Tutorial_MainMenuButtonWidth), NEXT_BUTTON_SIZE_Y)))
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
	const bool completed = tutorial->m_steps[currentStep].m_completed;

	ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialButtonText));
	ImGui::PushStyleColor(ImGuiCol_Button, I3T::getUI()->getTheme().get(EColor::TutorialButtonBg));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getUI()->getTheme().get(EColor::TutorialButtonActive));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getUI()->getTheme().get(EColor::TutorialButtonHovered));
	if (currentStep < tutorial->getStepCount() - 1)
	{
		if (GUI::Button(_t("Next"), !completed, ImVec2(-1, NEXT_BUTTON_SIZE_Y)))
		{
			TutorialManager::instance().setStep(currentStep + 1);
			// std::cout << m_currentStep << std::endl;
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}
	}
	else
	{
		if (GUI::Button(_t("Finish"), !completed, ImVec2(-1, NEXT_BUTTON_SIZE_Y)))
		{
			TutorialManager::instance().setTutorial(nullptr);
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
	ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::TutorialBold));
	ImGui::Markdown(headline->m_content.c_str(), headline->m_content.length(), m_mdConfig);
	ImGui::SetWindowFontScale(1.f);
	ImGui::PopFont();
}

static void renderTestQuestionControls(TestQuestion* task)
{
	if (I3TGui::ButtonWithLog(_t("Submit")))
	{
		task->submit();
	}

	if (task->m_isSubmitted)
	{
		if (!task->m_isCorrect)
		{
			ImGui::TextUnformatted(_t("Incorrect!"));
		}
		else
		{
			ImGui::TextUnformatted(_t("Correct!"));
		}
	}
}

void TutorialWindow::renderChoiceTask(ChoiceTask* choice)
{
	ImGui::PushID(choice->m_content.c_str());

	ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE));
	ImGui::TextUnformatted(choice->m_content.c_str());

	ImGui::BeginDisabled(choice->m_isCorrect);

	for (int i = 0; i < choice->m_choices.size(); i++)
	{
		ImGui::RadioButton(choice->m_choices[i].c_str(), &choice->m_selected, i);
	}

	ImGui::EndDisabled();

	renderTestQuestionControls(choice);

	ImGui::PopID();
}

void TutorialWindow::renderMultiChoiceTask(MultiChoiceTask* multiChoice)
{
	ImGui::PushID(multiChoice->m_content.c_str());

	ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE));
	ImGui::TextUnformatted(multiChoice->m_content.c_str());

	ImGui::BeginDisabled(multiChoice->m_isCorrect);

	for (int i = 0; i < multiChoice->m_choices.size(); i++)
	{
		bool value = multiChoice->m_selected[i];
		if (ImGui::Checkbox(multiChoice->m_choices[i].c_str(), &value))
		{
			multiChoice->m_selected[i] = value;
		}
	}

	ImGui::EndDisabled();

	renderTestQuestionControls(multiChoice);

	ImGui::PopID();
}

void TutorialWindow::renderInputTask(InputTask* input)
{
	ImGui::PushID(input->m_content.c_str());

	ImGui::Dummy(ImVec2(0.0f, SIMPLE_SPACE));
	ImGui::TextUnformatted(input->m_content.c_str());

	ImGui::BeginDisabled(input->m_isCorrect);

	constexpr auto size = sizeof(input->m_input);
	ImGui::InputText("##Answer", &input->m_input[0], size);

	ImGui::SameLine();

	ImGui::EndDisabled();

	renderTestQuestionControls(input);

	ImGui::PopID();
}

void TutorialWindow::renderTask(Task* task)
{
	// Calculate font size multiplier relative to default imgui fontsize (10) minus space between paragraphs
	float fontMult = (ImGui::GetFontSize() - 1) / 10;
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
	if (!task->m_completed)
	{
		ImVec2 drawPos =
		    ImGui::GetCursorScreenPos() +
		    ImVec2(I3T::getUI()->getTheme().get(ESize::Tutorial_TaskSquareXPadding), ImGui::GetStyle().FramePadding.y);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImU32 color = ImGui::ColorConvertFloat4ToU32(I3T::getUI()->getTheme().get(EColor::TutorialTitleText));
		draw_list->AddRectFilled(ImVec2(drawPos.x, drawPos.y), ImVec2(drawPos.x + squareSize, drawPos.y + squareSize),
		                         color);
		ImGui::Dummy(ImVec2(squareSize + I3T::getUI()->getTheme().get(ESize::Tutorial_TaskSquareXPadding), squareSize));
		ImGui::SameLine();
	}
	else
	{
		ImGui::Dummy(ImVec2(I3T::getUI()->getTheme().get(ESize::Tutorial_TaskSquareXPadding) / 2, squareSize));
		ImGui::SameLine();
		ImGui::TextUnformatted(ICON_I3T_CHECK);
		ImGui::SameLine();
	}

	// Load task font and color
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, I3T::getUI()->getTheme().get(EColor::TutorialHighlightText));
	ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::TutorialBold));

	// Print text from markdown
	ImGui::Markdown(task->m_content.c_str(), task->m_content.length(), m_mdConfig);

	ImGui::PopStyleColor();
	ImGui::PopFont(); // velikost zpet
}

void TutorialWindow::renderHint(Hint* hint)
{
	ImGui::Dummy(ImVec2(0.0f, SMALL_SPACE));

	ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::TutorialBold));
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

	// Hint button, pass the hint content to create a unique id
	if (I3TGui::ButtonWithLog(fmt::format("Tip##{}", hint->m_content).c_str(),
	                          ImVec2(TIP_BUTTON_SIZE_X, TIP_BUTTON_SIZE_Y)))
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

inline ImGui::MarkdownImageData TutorialWindow::ImageCallback(ImGui::MarkdownLinkCallbackData data_)
{
	std::string imageFilename(data_.link, data_.linkLength);

	// try to find the texture, if it isn't loaded, then load it
	std::shared_ptr<GUIImage> img = nullptr;

	auto tutorial = TutorialManager::instance().getTutorial();

	if (tutorial != nullptr)
	{
		// image already loaded
		if (tutorial->m_filenameToImage.contains(imageFilename))
		{
			img = tutorial->m_filenameToImage[imageFilename];
		}
		// image not yet loaded
		else
		{
			LOG_INFO("Loading image " + imageFilename);
			img =
			    TutorialLoader::loadImage(TutorialLoader::getDirectory(tutorial->m_header->m_filename) + imageFilename);
			tutorial->m_filenameToImage[imageFilename] = img; // save for future use
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
