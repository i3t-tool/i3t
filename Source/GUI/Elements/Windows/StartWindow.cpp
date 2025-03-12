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
#include "StartWindow.h"

#include <filesystem>

#include "Core/Input/InputManager.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "Config.h"
#include "Core/Resources/ResourceManager.h"

#include "GUI/Elements/MainMenuBar.h"
#include "GUI/Elements/Modals/BeforeNewTutModal.h"
#include "GUI/IconFonts/Icons.h"
#include "GUI/UIModule.h"
#include "I3T.h"
#include "Logger/Logger.h"
#include "State/StateManager.h"
#include "Tutorial/TutorialLoader.h"
#include "Tutorial/TutorialManager.h"
#include "UserData.h"
#include "Utils/Other.h"

StartWindow::StartWindow(bool show) : IWindow(ICON_I3T_HOME " Welcome", show)
{
	// load images
	try
	{
		m_dummyImage = std::make_shared<GUIImage>(Config::TEXTURE_FOLDER + "dummy.png");
		m_folderImage = std::make_shared<GUIImage>(Config::TEXTURE_FOLDER + "pilkaFolder.png");
		m_cvutImage = std::make_shared<GUIImage>(Config::TEXTURE_FOLDER + "cvutLogo.png");
		m_i3tImage = std::make_shared<GUIImage>(Config::TEXTURE_FOLDER + "logoi3t.png");
	}
	catch (std::runtime_error& e)
	{
		LOG_ERROR(e.what());
	}

	TutorialManager::instance().reloadTutorials();
}

extern void beforeExitScene();

/**
 * \brief Render the logo and the I3T brief description
 */
void StartWindow::renderLeftPanel() const
{
	// PFx temporary

	// Styling constants todo move all constants here, possibly load from theme or
	// other styling settings
	const float titleVerticalOffset = I3T::getUI()->getTheme().get(ESize::StartWindow_TitleVerticalOffset);
	const float leftBarWidth = I3T::getUI()->getTheme().get(ESize::StartWindow_LeftBarWidth);

	// LEFT CHILD WINDOW
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
	                    I3T::getUI()->getTheme().get(ESizeVec2::StartWindow_LeftWinPadding));
	ImGui::BeginChild("left", ImVec2(leftBarWidth, 0), false,
	                  ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar |
	                      ImGuiWindowFlags_NoScrollWithMouse);
	{
		// LOGO
		ImGui::Dummy(ImVec2(0, titleVerticalOffset / 2));
		ImGui::Image((ImTextureID) (intptr_t) m_i3tImage->m_texID,
		             ImVec2(m_i3tImage->m_width / 2, m_i3tImage->m_height / 2));

		// TITLE
		ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::WelcomeTitle));
		ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::StartWindow_TitleFont));
		ImGui::Text("Learn\nTransformations");
		ImGui::PopStyleColor();
		ImGui::PopFont();

		ImGui::Dummy(ImVec2(0, 10));
		// DOTS
		float size = I3T::getUI()->getTheme().get(ESize::StartWindow_DotSize);
		float space = I3T::getUI()->getTheme().get(ESize::StartWindow_DotSpacing);
		ImVec2 screenPos = ImGui::GetCursorScreenPos();
		float x = screenPos.x;
		float y = screenPos.y;
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		for (int i = 0; i < 3; i++)
		{
			auto dotColor =
			    ImGui::ColorConvertFloat4ToU32(I3T::getUI()->getTheme().get(EColor::StartWindow_NewSceneButton));
			draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + size, y + size), dotColor);
			x += size + space;
		}
		ImGui::Dummy(ImVec2(0, size));

		ImGui::Dummy(ImVec2(0, 10));
		// DESCRIPTION
		ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::WelcomeDescription));
		ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::StartWindow_DescriptionFont));
		ImGui::TextWrapped("I3T is an educational application that allows the "
		                   "study of 3D transformations and their "
		                   "hierarchy in an illustrative way.");
		ImGui::PopStyleColor();
		ImGui::PopFont();

		// IMAGE
		if (m_cvutImage)
		{
			ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - m_cvutImage->m_height);
			ImGui::Image((ImTextureID) (intptr_t) m_cvutImage->m_texID,
			             ImVec2(m_cvutImage->m_width, m_cvutImage->m_height));
		}
		ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - m_cvutImage->m_height);
		ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - m_cvutImage->m_height);

		ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::Button));
		ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialButtonText));
		ImGui::PushStyleColor(ImGuiCol_Button, I3T::getUI()->getTheme().get(EColor::TutorialButtonBg));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getUI()->getTheme().get(EColor::TutorialButtonActive));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getUI()->getTheme().get(EColor::TutorialButtonHovered));

		const auto language = TutorialManager::instance().getLanguage();
		GLuint flagCz = I3T::getResourceManager().texture("Data/Textures/flags/cz.png");
		GLuint flagEn = I3T::getResourceManager().texture("Data/Textures/flags/gb.png");
		GLuint flag = language == ETutorialLanguage::Czech ? flagCz : flagEn;
		if (ImGui::ImageButton("##LangButton", (void*) (intptr_t) flag, ImVec2(48, 36), ImVec2(0., 0.), ImVec2(1., 1.),
		                       ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
		{
			TutorialManager::instance().toggleLanguage();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		ImGui::PopStyleColor(4);
		ImGui::PopFont();

		ImGui::EndChild();
	}
	ImGui::PopStyleVar();
}


void StartWindow::renderRightPanel()
{
	// Styling constants todo move all constants here, possibly load from theme or
	// other styling settings
	const float loadBtnWidth = I3T::getUI()->getTheme().get(ESize::StartWindow_LoadButtonWidth);
	const float startNewBtnWidth = loadBtnWidth;
	const float buttonHeight = I3T::getUI()->getTheme().get(ESize::StartWindow_ButtonHeight);
	const float thumbImageSize = I3T::getUI()->getTheme().get(ESize::StartWindow_ThumbImageSize);
	const float startBtnWidth = I3T::getUI()->getTheme().get(ESize::StartWindow_StartButtonWidth);

	// RIGHT CHILD WINDOW
	const ImVec2 outerPadding = I3T::getUI()->getTheme().get(ESizeVec2::StartWindow_RightWinOuterPadding);
	const ImVec2 innerPadding = I3T::getUI()->getTheme().get(ESizeVec2::StartWindow_RightWinInnerPadding);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, outerPadding);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, I3T::getUI()->getTheme().get(EColor::StartWindow_WindowBackground));
	ImGui::BeginChild("panel", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		// YOUR SCENE CHILD WINDOW
		if (m_finishedAllTutorials)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, innerPadding);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,
			                    I3T::getUI()->getTheme().get(ESize::StartWindow_YourSceneWinRounding));
			ImGui::PushStyleColor(ImGuiCol_ChildBg,
			                      I3T::getUI()->getTheme().get(EColor::StartWindow_YourSceneWinBackground));
			ImGui::BeginChild("header", ImVec2(0, thumbImageSize + 2 * innerPadding.y), true,
			                  ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollWithMouse |
			                      ImGuiWindowFlags_NoScrollbar);
			{
				// FOLDER IMAGE
				if (m_folderImage)
				{
					ImGui::Image((ImTextureID) (intptr_t) m_folderImage->m_texID,
					             ImVec2(thumbImageSize, thumbImageSize));
				}
				ImGui::SameLine();

				// YOUR SCENE TEXT
				ImGui::BeginGroup();
				{
					ImGui::Dummy(ImVec2(0, 10));
					ImGui::PushStyleColor(ImGuiCol_Text,
					                      I3T::getUI()->getTheme().get(EColor::StartWindow_DescriptionFont));
					ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::WelcomeItemTitle));
					ImGui::Text("Your scene");
					ImGui::PopFont();
					ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::WelcomeItemDescription));
					ImGui::Text("Start with an empty scene or open your previous work.");
					ImGui::PopFont();
					ImGui::PopStyleColor();
				}
				ImGui::EndGroup();

				ImGui::SameLine(ImGui::GetContentRegionMax().x - loadBtnWidth);

				// BUTTONS
				ImGui::BeginGroup();
				{
					ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::Button));
					ImGui::PushStyleColor(ImGuiCol_Text,
					                      I3T::getUI()->getTheme().get(EColor::StartWindow_NewSceneButtonFont));
					ImGui::PushStyleColor(ImGuiCol_Button,
					                      I3T::getUI()->getTheme().get(EColor::StartWindow_NewSceneButton));
					if (ImGui::Button("New", ImVec2(startNewBtnWidth, buttonHeight)))
					{
						this->hide();
						InputManager::triggerAction("new", EKeyState::Pressed);
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					}
					ImGui::Dummy(ImVec2(0, 2));
					if (ImGui::Button("Open", ImVec2(loadBtnWidth, buttonHeight)))
					{
						if (MenuBarDialogs::open())
						{
							this->hide();
						}
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					}
					ImGui::PopStyleColor(2);
					ImGui::PopFont();
				}
				ImGui::EndGroup();
			}
			ImGui::EndChild(); // SCENE PICKER
			ImGui::PopStyleColor();
			ImGui::PopStyleVar(2);

			// Recent scenes
			ImGui::Dummy(ImVec2(0, 10));

			// Headline for Recent section
			ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::WelcomeTitle)); // Use a larger font for the headline
			ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(
			                                         EColor::StartWindow_DescriptionFont)); // Depending on the theme
			ImGui::Text("Recent");                                                          // Headline text
			ImGui::PopStyleColor();
			ImGui::PopFont();

			// Spacer below headline
			ImGui::Dummy(ImVec2(0, 5));

			// Show recent files (limit to 3)
			const auto& recentFiles = getUserData().recentFiles;

			if (recentFiles.empty())
			{
				ImGui::Text("No recent files");
			}
			else
			{
				const float buttonWidth = I3T::getUI()->getTheme().get(ESize::StartWindow_StartButtonWidth);
				const float buttonHeight = I3T::getUI()->getTheme().get(ESize::StartWindow_ButtonHeight);
				const float spacing = 20.0f;      // Larger space between text and button
				const float paddingRight = 10.0f; // Add padding from the right side
				const float availableWidth = ImGui::GetContentRegionAvail().x;

				int count = 0;
				for (auto it = recentFiles.rbegin(); it != recentFiles.rend(); ++it)
				{
					if (count >= 3)
					{
						break;
					}
					count++;
					ImGui::BeginGroup();

					// Calculate column widths
					const float textWidth = availableWidth - buttonWidth - spacing - paddingRight;

					// Column 1: File name and path
					ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::StartWindow_TitleFont));
					ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::WelcomeItemTitle));
					ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + textWidth);
					ImGui::Text(it->filename().string().c_str());
					ImGui::PopTextWrapPos();
					ImGui::PopFont();
					ImGui::PopStyleColor();

					ImGui::PushStyleColor(ImGuiCol_Text,
					                      I3T::getUI()->getTheme().get(EColor::StartWindow_DescriptionFont));
					ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::WelcomeItemDescription));
					ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + textWidth);
					ImGui::TextWrapped(it->string().c_str()); // File path
					ImGui::PopTextWrapPos();
					ImGui::PopFont();
					ImGui::PopStyleColor();

					// Column 2: Button (Aligned Right with Padding)
					ImGui::SameLine(availableWidth - buttonWidth - paddingRight);

					ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::Button));
					ImGui::PushStyleColor(ImGuiCol_Text,
					                      I3T::getUI()->getTheme().get(EColor::StartWindow_NewSceneButtonFont));
					ImGui::PushStyleColor(ImGuiCol_Button,
					                      I3T::getUI()->getTheme().get(EColor::StartWindow_NewSceneButton));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
					                      I3T::getUI()->getTheme().get(EColor::TutorialButtonHovered));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive,
					                      I3T::getUI()->getTheme().get(EColor::TutorialButtonActive));

					if (ImGui::Button(("Open##" + it->string()).c_str(), ImVec2(buttonWidth, buttonHeight)))
					{
						this->hide(); // Close Welcome window
						askBeforeExitScene([scenePath = *it]() {
							App::getModule<StateManager>().loadScene(scenePath);
						});
					}

					ImGui::PopStyleColor(4);
					ImGui::PopFont();

					ImGui::EndGroup();
					ImGui::Dummy(ImVec2(0, 10)); // Spacer between entries
				}
			}
		}
		// Add a small spacer below "Your Scene"
		ImGui::Dummy(ImVec2(0, 10));

		// Headline for tutorials
		ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::WelcomeTitle));
		ImGui::PushStyleColor(
		    ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::StartWindow_DescriptionFont)); // Depending on the theme
		ImGui::Text("Tutorials");
		ImGui::PopStyleColor();
		ImGui::PopFont();

		// Add a small spacer below the headline
		ImGui::Dummy(ImVec2(0, 5));

		// THE LIST OF TUTORIALS
		for (auto& header : TutorialManager::instance().getTutorialHeaders())
		{
			// ITEM
			ImGui::BeginGroup();
			{
				ImGui::Indent(innerPadding.x);

				// THUMBNAIL IMAGE
				auto img = header->m_thumbnailImage;
				if (img)
				{
					ImGui::Image((ImTextureID) (intptr_t) img->m_texID, ImVec2(thumbImageSize, thumbImageSize));
				}
				else
				{
					// todo load dummy at introwindow init
					if (m_dummyImage)
					{
						ImGui::Image((ImTextureID) (intptr_t) m_dummyImage->m_texID,
						             ImVec2(thumbImageSize, thumbImageSize));
					}
					else
					{
						ImGui::Image(nullptr, ImVec2(thumbImageSize, thumbImageSize));
					}
				}

				ImGui::SameLine();
				std::string descChildName = "Desc##" + header->m_filename.string();
				ImVec2 descSize(ImGui::GetContentRegionAvail().x, thumbImageSize);
				descSize.x -= (startNewBtnWidth + 2 * innerPadding.x + outerPadding.x);

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
				ImGui::BeginChild(descChildName.c_str(), descSize, false,
				                  ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
				{
					// TITLE
					ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::StartWindow_TitleFont));
					ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::WelcomeItemTitle));
					ImGui::TextWrapped(header->m_title.c_str());
					ImGui::PopStyleColor();
					ImGui::PopFont();
					// DESCRIPTIONS
					ImGui::PushStyleColor(ImGuiCol_Text,
					                      I3T::getUI()->getTheme().get(EColor::StartWindow_DescriptionFont));
					ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::WelcomeItemDescription));
					float predictedTextSize = ImGui::CalcTextSize(header->m_description.c_str(), nullptr, false,
					                                              ImGui::GetContentRegionAvail().x)
					                              .y;
					bool willTextFit = ImGui::GetContentRegionAvail().y - predictedTextSize >= 0;
					ImGui::TextWrapped(header->m_description.c_str());
					// Show tooltip when description doesn't fit
					if (!willTextFit && ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						{
							ImGui::PushTextWrapPos(ImGui::GetFontSize() * 20.0f);
							ImGui::PushStyleColor(ImGuiCol_Text,
							                      I3T::getUI()->getTheme().get(EColor::StartWindow_DescriptionFont));
							ImGui::TextUnformatted(header->m_description.c_str());
							ImGui::PopTextWrapPos();
							ImGui::PopStyleColor();
						}
						ImGui::EndTooltip();
					}
					ImGui::PopStyleColor();
					ImGui::PopFont();
				}
				ImGui::EndChild();
				ImGui::PopStyleVar();

				ImGui::SameLine(ImGui::GetContentRegionMax().x - startNewBtnWidth - innerPadding.x - outerPadding.x);
				// START BUTTON
				ImGui::BeginGroup();
				{
					ImGui::PushFont(I3T::getUI()->getTheme().get(EFont::Button));
					ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialButtonText));
					ImGui::PushStyleColor(ImGuiCol_Button, I3T::getUI()->getTheme().get(EColor::TutorialButtonBg));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive,
					                      I3T::getUI()->getTheme().get(EColor::TutorialButtonActive));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
					                      I3T::getUI()->getTheme().get(EColor::TutorialButtonHovered));
					std::string buttonName = "Start##" + header->m_filename.string();
					if (ImGui::Button(buttonName.c_str(), ImVec2(startBtnWidth, buttonHeight)))
					{
						auto tutorial = TutorialLoader::loadTutorial(header);
						if (tutorial)
						{
							loadTutorialAndShowWindow(header, tutorial);
						}
						else
						{
							LOG_INFO("ERR: Tutorial " + header->m_title + " not loaded");
						}
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					}
					ImGui::PopStyleColor(4);
					ImGui::PopFont();
				}
				ImGui::EndGroup();
			}
			ImGui::EndGroup();
		}

		ImGui::EndChild(); // right panel
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}

void StartWindow::render()
{
	// PFx temporary

	// Styling constants todo move all constants here, possibly load from theme or
	// other styling settings
	const float minWinWidth = I3T::getUI()->getTheme().get(ESize::StartWindow_WinWidth);
	const float minWinHeight = I3T::getUI()->getTheme().get(ESize::StartWindow_WinHeight);

	// const ImVec2 logoOffset = I3T::getUI()->getTheme().get(ESizeVec2::StartWindow_LogoOffset);

	// Set initial window size
	ImVec2 windowSize = I3T::getUI()->getTheme().get(ESizeVec2::StartWindow_WinSize);
	static bool firstTime = true;
	if (firstTime)
	{
		firstTime = false;
		ImGui::SetNextWindowSize(windowSize);
		ImGui::SetNextWindowPos(ImVec2(1000.0f, 500.0f), 0,
		                        ImVec2(0.5f, 0.5f)); // \todo set pos to the main window center
	}
	// todo change all color specifications to
	// I3T::getUI()->getTheme().get(EColor::DesiredColor) WINDOW
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, I3T::getUI()->getTheme().get(ESizeVec2::StartWindow_WinPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, I3T::getUI()->getTheme().get(ESize::StartWindow_WinRounding));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, I3T::getUI()->getTheme().get(ESize::StartWindow_FrameRounding));
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, I3T::getUI()->getTheme().get(ESize::StartWindow_ScrollbarSize));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(minWinWidth, minWinHeight));
	// ImGui::PushStyleColor(ImGuiCol_TitleBg,
	// I3T::getUI()->getTheme().get(EColor::TutorialBgColor));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, I3T::getUI()->getTheme().get(EColor::StartWindow_DescriptionBackground));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, I3T::getUI()->getTheme().get(EColor::StartWindow_ScrollbarBackground));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, I3T::getUI()->getTheme().get(EColor::StartWindow_ScrollbarGrab));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered,
	                      I3T::getUI()->getTheme().get(EColor::StartWindow_ScrollbarGrabHovered));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive,
	                      I3T::getUI()->getTheme().get(EColor::StartWindow_ScrollbarGrabActive));
	ImGui::PushStyleColor(ImGuiCol_Separator, I3T::getUI()->getTheme().get(EColor::StartWindow_Separator));
	ImGui::PushStyleColor(ImGuiCol_Button, I3T::getUI()->getTheme().get(EColor::StartWindow_DefaultButton));


	ImGui::Begin(getName(), getShowPtr(),
	             ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
	                 ImGuiWindowFlags_NoSavedSettings); // | ImGuiWindowFlags_NoMove
	{
		this->updateWindowInfo();

		// LOGO I3T
		/* TODO: Restore original design
		ImVec2 logoPos = ImGui::GetWindowPos() + logoOffset;
		ImGui::GetForegroundDrawList()->AddImage((ImTextureID) m_i3tImage->m_texID, logoPos,
		                                         logoPos + ImVec2(m_i3tImage->m_width, m_i3tImage->m_height));
		*/
		renderLeftPanel();

		ImGui::SameLine();

		// FOR POSSIBLE DEVELOPMENT OF TABS - TO BE MOVED UNDER "YOUR SCENE"
		// ImGui::Spacing();
		//// TABS
		// if (ImGui::BeginTabBar("TabBar"))
		//{
		//   // TUTORIALS
		//   if (ImGui::BeginTabItem("Tutorials"))
		//   {
		//     // here move the tutorial list and wrap it in a scrollable window /
		//     remove scrolling from the upper window ImGui::EndTabItem();
		//   }
		//   // TEMPLATES
		//   if (ImGui::BeginTabItem("Templates"))
		//   {
		//     ImGui::Text("List of preset scene files");
		//     ImGui::EndTabItem();
		//   }
		//   // RECENT
		//   if (ImGui::BeginTabItem("Recent"))
		//   {
		//     ImGui::Text("List of recent files files");
		//     ImGui::EndTabItem();
		//   }
		//   ImGui::EndTabBar();
		// }

		renderRightPanel();

		ImGui::End(); // window
	}
	ImGui::PopStyleVar(5);
	ImGui::PopStyleColor(7);
}

void StartWindow::loadTutorialAndShowWindow(const Ptr<TutorialHeader>& header, Ptr<Tutorial> tut)
{
	I3T::getUI()->getWindowManager().showWindow(shared_from_this(), false);
	TutorialManager::instance().setTutorial(tut);
	m_windowManager->openModal<BeforeNewTutModal>();
	LOG_DEBUG("Tutorial " + header->m_title + " loaded");
}
