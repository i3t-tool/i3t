#include "StartWindow.h"

#include <filesystem>

#include "imgui.h"

#include "Config.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "WorkspaceWindow.h"
#include "imgui_internal.h"

#include "Commands/ApplicationCommands.h"
#include "Core/API.h"
#include "GUI/Elements/Dialogs/SystemDialogs.h"
#include "GUI/Elements/MainMenuBar.h"
#include "GUI/Elements/Modals/BeforeNewModal.h"
#include "GUI/Elements/Modals/BeforeNewTutModal.h"
#include "GUI/UIModule.h"
#include "Logger/Logger.h"
#include "Tutorial/TutorialLoader.h"
#include "Utils/Other.h"

StartWindow::StartWindow(bool show) : IWindow(show)
{
	// load images
	try
	{
		m_dummyImage = std::make_shared<GUIImage>(GUIImage(Config::TEXTURE_FOLDER + "dummy.png"));
	}
	catch (std::runtime_error& e)
	{
		LOG_ERROR(e.what())
	}
	try
	{
		m_folderImage = std::make_shared<GUIImage>(GUIImage(Config::TEXTURE_FOLDER + "pilkaFolder.png"));
	}
	catch (std::runtime_error& e)
	{
		LOG_ERROR(e.what())
	}
	try
	{
		m_cvutImage = std::make_shared<GUIImage>(GUIImage(Config::TEXTURE_FOLDER + "cvutLogo.png"));
	}
	catch (std::runtime_error& e)
	{
		LOG_ERROR(e.what())
	}
	try
	{
		m_i3tImage = std::make_shared<GUIImage>(GUIImage(Config::TEXTURE_FOLDER + "logoi3t.png"));
	}
	catch (std::runtime_error& e)
	{
		LOG_ERROR(e.what())
	}
	reloadTutorials();
}

void StartWindow::reloadTutorials()
{
	// preload all tutorials located in TUTORIALS_FOLDER recursively
	std::string path = Config::TUTORIALS_FOLDER;
	if (path[0] == '/')
	{
		path.erase(0, 1);
	}

	if (std::filesystem::exists(path))
	{
		LOG_INFO("Searching for tutorials in: " + path);
		// For all files in path recursively
		for (auto const& entry : std::filesystem::recursive_directory_iterator(
		         path, std::filesystem::directory_options::skip_permission_denied))
		{
			if (entry.path().extension() == ".tut")
			{
				std::string pathString = entry.path().string();
				LOG_INFO(pathString);
				// Load header part of tutorial
				if (std::shared_ptr<TutorialHeader> header = TutorialLoader::loadTutorialHeader(pathString); header)
				{
					m_tutorial_headers.push_back(std::move(header));
				}
				else
				{
					LOG_ERROR("Tutorial header " + pathString + " not loaded.");
				}
			}
		}
	}
	else
	{
		LOG_INFO("Path for tutorials not found. Searched path: " + path);
	}
}

/**
 * \brief Render the logo and the I3T brief description
 */
void StartWindow::renderLeftPanel() const
{
	// PFx temporary

	// Styling constants todo move all constants here, possibly load from theme or
	// other styling settings
	// const ImVec2 logoOffset = ImVec2(5, -20);
	const float titleVerticalOffset = App::get().getUI()->getTheme().get(ESize::StartWindow_TitleVerticalOffset);
	const float leftBarWidth = App::get().getUI()->getTheme().get(ESize::StartWindow_LeftBarWidth);

	//// LOGO I3T
	// ImVec2 logoPos = ImGui::GetWindowPos() + logoOffset;
	// ImGui::GetForegroundDrawList()->AddImage((ImTextureID) m_i3tImage->m_texID, logoPos,
	//                                         logoPos + ImVec2(m_i3tImage->m_width, m_i3tImage->m_height));

	// LEFT CHILD WINDOW
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
	                    App::get().getUI()->getTheme().get(ESizeVec2::StartWindow_LeftWinPadding));
	ImGui::BeginChild("left", ImVec2(leftBarWidth, 0), false,
	                  ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar |
	                      ImGuiWindowFlags_NoScrollWithMouse);
	{
		ImGui::Dummy(ImVec2(0, titleVerticalOffset));

		// TITLE
		ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::WelcomeTitle));
		ImGui::PushStyleColor(ImGuiCol_Text, App::get().getUI()->getTheme().get(EColor::StartWindow_TitleFont));
		ImGui::Text("Learn\nTransformations");
		ImGui::PopStyleColor();
		ImGui::PopFont();

		ImGui::Dummy(ImVec2(0, 10));
		// DOTS
		float size = App::get().getUI()->getTheme().get(ESize::StartWindow_DotSize);
		float space = App::get().getUI()->getTheme().get(ESize::StartWindow_DotSpacing);
		ImVec2 screenPos = ImGui::GetCursorScreenPos();
		float x = screenPos.x;
		float y = screenPos.y;
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		for (int i = 0; i < 3; i++)
		{
			auto dotColor =
			    ImGui::ColorConvertFloat4ToU32(App::get().getUI()->getTheme().get(EColor::StartWindow_NewSceneButton));
			draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + size, y + size), dotColor);
			x += size + space;
		}
		ImGui::Dummy(ImVec2(0, size));

		ImGui::Dummy(ImVec2(0, 10));
		// DESCRIPTION
		ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::WelcomeDescription));
		ImGui::PushStyleColor(ImGuiCol_Text, App::get().getUI()->getTheme().get(EColor::StartWindow_DescriptionFont));
		ImGui::TextWrapped("I3T is an educational application that allows the "
		                   "study of 3D transformations and their "
		                   "hierarchy in an illustrative way.");
		ImGui::PopStyleColor();
		ImGui::PopFont();

		// IMAGE
		if (m_cvutImage)
		{
			ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - m_cvutImage->m_height);
			ImGui::Image((ImTextureID) m_cvutImage->m_texID, ImVec2(m_cvutImage->m_width, m_cvutImage->m_height));
		}

		ImGui::EndChild();
	}
	ImGui::PopStyleVar();
}

void StartWindow::renderRightPanel()
{

	// PFx temporary

	// Styling constants todo move all constants here, possibly load from theme or
	// other styling settings
	const float loadBtnWidth = App::get().getUI()->getTheme().get(ESize::StartWindow_LoadButtonWidth);
	const float startNewBtnWidth = loadBtnWidth;
	const float buttonHeight = App::get().getUI()->getTheme().get(ESize::StartWindow_ButtonHeight);
	const float thumbImageSize = App::get().getUI()->getTheme().get(ESize::StartWindow_ThumbImageSize);
	const float startBtnWidth = App::get().getUI()->getTheme().get(ESize::StartWindow_StartButtonWidth);


	// RIGHT CHILD WINDOW
	const ImVec2 outerPadding = App::get().getUI()->getTheme().get(ESizeVec2::StartWindow_RightWinOuterPadding);
	const ImVec2 innerPadding = App::get().getUI()->getTheme().get(ESizeVec2::StartWindow_RightWinInnerPadding);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, outerPadding);
	// ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12);
	ImGui::PushStyleColor(ImGuiCol_ChildBg,
	                      Application::get().getUI()->getTheme().get(EColor::StartWindow_WindowBackground));
	ImGui::BeginChild("panel", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		// YOUR SCENE CHILD WINDOW
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, innerPadding);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,
		                    App::get().getUI()->getTheme().get(ESize::StartWindow_YourSceneWinRounding));
		ImGui::PushStyleColor(ImGuiCol_ChildBg,
		                      App::get().getUI()->getTheme().get(EColor::StartWindow_YourSceneWinBackground));
		ImGui::BeginChild("header", ImVec2(0, thumbImageSize + 2 * innerPadding.y), true,
		                  ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollWithMouse |
		                      ImGuiWindowFlags_NoScrollbar);
		{
			// FOLDER IMAGE
			if (m_folderImage)
			{
				// ImVec2 offset = ImVec2(ImGui::GetCursorPosX() + (thumbImageSize -
				// folderImage->m_width)/2,ImGui::GetCursorPosY() + (thumbImageSize -
				// folderImage->m_height)/2); ImGui::Dummy(ImVec2(thumbImageSize,
				// thumbImageSize)); ImGui::SetCursorPos(offset);
				ImGui::Image((ImTextureID) m_folderImage->m_texID, ImVec2(thumbImageSize, thumbImageSize));
			}
			else
			{}
			ImGui::SameLine();
			// YOUR SCENE TEXT
			// ImGui::AlignTextToFramePadding();
			ImGui::BeginGroup();
			{
				// ImGui::BeginVertical("yourScene", ImVec2(0, 0));
				// ImGui::Spring(1);  // not working

				ImGui::Dummy(ImVec2(0, 10));
				ImGui::PushStyleColor(ImGuiCol_Text,
				                      App::get().getUI()->getTheme().get(EColor::StartWindow_DescriptionFont));
				ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::WelcomeItemTitle));
				ImGui::Text("Your scene");
				ImGui::PopFont();
				ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::WelcomeItemDescription));
				ImGui::Text("Start with an empty scene or open your previous work.");
				ImGui::PopFont();
				ImGui::PopStyleColor();

				// ImGui::Spring(1);  // not working
				// ImGui::EndVertical();
				ImGui::EndGroup();
			}
			// ImGui::GetFontSize()
			// ImGui::SetNextItemWidth(100.0f);
			// const float itemSpacing = ImGui::GetStyle().

			ImGui::SameLine(ImGui::GetContentRegionMax().x - loadBtnWidth);

			// BUTTONS
			ImGui::BeginGroup();
			{
				// ImGui::BeginVertical("buttons", ImVec2(0, 0));
				// ImGui::Spring(1);

				ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::Button));
				ImGui::PushStyleColor(ImGuiCol_Text,
				                      App::get().getUI()->getTheme().get(EColor::StartWindow_NewSceneButtonFont));
				ImGui::PushStyleColor(ImGuiCol_Button,
				                      App::get().getUI()->getTheme().get(EColor::StartWindow_NewSceneButton));
				if (ImGui::Button("New", ImVec2(startNewBtnWidth, buttonHeight)))
				{
					this->hide();
					App::getModule<UIModule>().openModal<BeforeNewModal>();
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				}
				// ImGui::SameLine();
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

				// ImGui::Spring(1);
				// ImGui::EndVertical();
				ImGui::EndGroup();
			}
			ImGui::EndChild(); // SCENE PICKER
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		ImGui::Dummy(ImVec2(0, 5));

		// THE LIST OF TUTORIALS
		for (auto& header : m_tutorial_headers)
		{
			// ITEM
			ImGui::BeginGroup();
			{
				ImGui::Indent(innerPadding.x);
				const float titleDescWidth = ImGui::GetContentRegionAvail().x - (thumbImageSize + startBtnWidth);

				// ImGui::Columns(3, "ThreeCols", false);
				// ImGui::SetColumnWidth(0, thumbImageSize + 2 *
				// ImGui::GetStyle().ColumnsMinSpacing); ImGui::SetColumnWidth(1,
				// titleDescWidth - 2 * ImGui::GetStyle().ColumnsMinSpacing);
				// ImGui::SetColumnWidth(2, startBtnWidth);

				// THUMBNAIL IMAGE
				auto img = header->m_thumbnailImage;
				if (img)
				{
					ImGui::Image((ImTextureID) img->m_texID, ImVec2(thumbImageSize, thumbImageSize));
				}
				else
				{
					// todo load dummy at introwindow init
					if (m_dummyImage)
					{
						ImGui::Image((ImTextureID) m_dummyImage->m_texID, ImVec2(thumbImageSize, thumbImageSize));
					}
					else
					{
						ImGui::Image(nullptr, ImVec2(thumbImageSize, thumbImageSize));
					}
				}

				// ImGui::NextColumn();
				// ImGui::SameLine(ImGui::GetCursorPosX() + innerPadding.x);
				ImGui::SameLine();
				std::string descChildName = "Desc##" + header->m_filename;
				ImVec2 descSize(ImGui::GetContentRegionAvailWidth(), thumbImageSize);
				descSize.x -= (startNewBtnWidth + 2 * innerPadding.x + outerPadding.x);
				// ImGui::PushClipRect(ImGui::GetCursorScreenPos(), descBottomRight,
				// true); ImGui::BeginGroup();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
				ImGui::BeginChild(descChildName.c_str(), descSize, false,
				                  ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
				{
					// TITLE
					ImGui::PushStyleColor(ImGuiCol_Text,
					                      App::get().getUI()->getTheme().get(EColor::StartWindow_TitleFont));
					ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::WelcomeItemTitle));
					ImGui::TextWrapped(header->m_title.c_str());
					ImGui::PopStyleColor();
					ImGui::PopFont();
					// DESCRIPTIONS
					ImGui::PushStyleColor(ImGuiCol_Text,
					                      App::get().getUI()->getTheme().get(EColor::StartWindow_DescriptionFont));
					ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::WelcomeItemDescription));
					float predictedTextSize = ImGui::CalcTextSize(header->m_description.c_str(), nullptr, false,
					                                              ImGui::GetContentRegionAvail().x)
					                              .y;
					bool willTextFit = ImGui::GetContentRegionAvail().y - predictedTextSize >= 0;
					// std::string debug = fmt::format("{} - {} = {}",
					// ImGui::GetContentRegionAvail().y, predictedTextSize,
					// willTextFit); ImGui::Text(debug.c_str());
					ImGui::TextWrapped(header->m_description.c_str());
					// show tooltip when description doesnt fit
					if (!willTextFit && ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						{
							ImGui::PushTextWrapPos(ImGui::GetFontSize() * 20.0f);
							ImGui::PushStyleColor(
							    ImGuiCol_Text, App::get().getUI()->getTheme().get(EColor::StartWindow_DescriptionFont));
							ImGui::TextUnformatted(header->m_description.c_str());
							ImGui::PopTextWrapPos();
							ImGui::PopStyleColor();
							ImGui::EndTooltip();
						}
					}
					ImGui::PopStyleColor();
					ImGui::PopFont();
					// ImGui::EndGroup();
					// ImGui::PopClipRect();
					ImGui::EndChild();
				}
				ImGui::PopStyleVar();

				// ImGui::NextColumn();
				ImGui::SameLine(ImGui::GetContentRegionMax().x - startNewBtnWidth - innerPadding.x - outerPadding.x);
				// START BUTTON
				ImGui::BeginGroup();
				{
					// ImGui::BeginVertical("start button", ImVec2(0, thumbImageSize));
					// ImGui::Spring(1);
					ImGui::PushFont(Application::get().getUI()->getTheme().get(EFont::Button));
					ImGui::PushStyleColor(ImGuiCol_Text,
					                      App::get().getUI()->getTheme().get(EColor::TutorialButtonText));
					ImGui::PushStyleColor(ImGuiCol_Button,
					                      App::get().getUI()->getTheme().get(EColor::TutorialButtonBg));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive,
					                      App::get().getUI()->getTheme().get(EColor::TutorialButtonActive));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
					                      App::get().getUI()->getTheme().get(EColor::TutorialButtonHovered));
					std::string buttonName = "Start##" + header->m_filename;
					if (ImGui::Button(buttonName.c_str(), ImVec2(startBtnWidth, buttonHeight)))
					{
						// TUTORIAL LOADING !!!
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
					// ImGui::Spring(1);
					// ImGui::EndVertical();
					ImGui::EndGroup();
				}
				// ImGui::Columns(1); // end columns
				ImGui::Dummy(ImVec2(0, 2));
				// CUSTOM SEPARATOR
				float thickness_draw = 1.0f;
				float thickness_layout = 0.0f;
				ImVec2 screenPos = ImGui::GetCursorScreenPos();
				const ImRect bb(screenPos, ImVec2(screenPos.x + ImGui::GetContentRegionAvailWidth() - innerPadding.x,
				                                  screenPos.y + thickness_draw));
				ImGui::ItemSize(ImVec2(0.0f, thickness_layout));
				if (ImGui::ItemAdd(bb, 0))
				{
					// Draw
					ImGui::GetWindowDrawList()->AddLine(bb.Min, ImVec2(bb.Max.x, bb.Min.y),
					                                    ImGui::GetColorU32(ImGuiCol_Separator));
				}
				// ImGui::Separator();
				ImGui::Dummy(ImVec2(0, 2));
				ImGui::EndGroup();
			}
			// ITEM ACTIONS
			// if (ImGui::IsItemHovered()) {
			// ImGui::BeginTooltip();
			// ImGui::Text("yay");
			// ImGui::EndTooltip();
			//}
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
	const float minWinWidth = App::get().getUI()->getTheme().get(ESize::StartWindow_WinWidth);
	const float minWinHeight = App::get().getUI()->getTheme().get(ESize::StartWindow_WinHeight);

	const ImVec2 logoOffset = App::get().getUI()->getTheme().get(ESizeVec2::StartWindow_LogoOffset);

	// Set initial window size
	ImVec2 windowSize = App::get().getUI()->getTheme().get(ESizeVec2::StartWindow_WinSize);
	static bool firstTime = true;
	if (firstTime)
	{
		firstTime = false;
		ImGui::SetNextWindowSize(windowSize);
		ImGui::SetNextWindowPos(ImVec2(1000.0f, 500.0f), 0,
		                        ImVec2(0.5f, 0.5f)); // \todo set pos to the main window center
	}
	// todo change all color specifications to
	// Application::get().getUI()->getTheme().get(EColor::DesiredColor) WINDOW
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
	                    App::get().getUI()->getTheme().get(ESizeVec2::StartWindow_WinPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,
	                    App::get().getUI()->getTheme().get(ESize::StartWindow_WinRounding));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,
	                    App::get().getUI()->getTheme().get(ESize::StartWindow_FrameRounding));
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize,
	                    App::get().getUI()->getTheme().get(ESize::StartWindow_ScrollbarSize));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(minWinWidth, minWinHeight));
	// ImGui::PushStyleColor(ImGuiCol_TitleBg,
	// Application::get().getUI()->getTheme().get(EColor::TutorialBgColor));
	ImGui::PushStyleColor(ImGuiCol_WindowBg,
	                      App::get().getUI()->getTheme().get(EColor::StartWindow_DescriptionBackground));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg,
	                      Application::get().getUI()->getTheme().get(EColor::StartWindow_ScrollbarBackground));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab,
	                      App::get().getUI()->getTheme().get(EColor::StartWindow_ScrollbarGrab));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered,
	                      App::get().getUI()->getTheme().get(EColor::StartWindow_ScrollbarGrabHovered));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive,
	                      App::get().getUI()->getTheme().get(EColor::StartWindow_ScrollbarGrabActive));
	ImGui::PushStyleColor(ImGuiCol_Separator, App::get().getUI()->getTheme().get(EColor::StartWindow_Separator));
	ImGui::PushStyleColor(ImGuiCol_Button, App::get().getUI()->getTheme().get(EColor::StartWindow_DefaultButton));
	ImGui::PushStyleColor(ImGuiCol_Text, App::get().getUI()->getTheme().get(EColor::StartWindow_DefaultButton));


	ImGui::Begin(setName("").c_str(), getShowPtr(),
	             ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
	                 ImGuiWindowFlags_NoSavedSettings); // | ImGuiWindowFlags_NoMove
	{
		this->updateWindowInfo();

		// LOGO I3T
		ImVec2 logoPos = ImGui::GetWindowPos() + logoOffset;
		ImGui::GetForegroundDrawList()->AddImage((ImTextureID) m_i3tImage->m_texID, logoPos,
		                                         logoPos + ImVec2(m_i3tImage->m_width, m_i3tImage->m_height));

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
	ImGui::PopStyleColor(8);
}

void StartWindow::loadTutorialAndShowWindow(Ptr<TutorialHeader> header, Ptr<Tutorial> tut)
{
	I3T::getUI()->getWindowManager().showWindow(shared_from_this(), false);
	setTutorial(tut);
	m_windowManager->openModal<BeforeNewTutModal>();
	LOG_DEBUG("Tutorial " + header->m_title + " loaded");
}

void StartWindow::showTutorialPopup()
{
	if (!App::getModule<StateManager>().isDirty())
	{
		popupActive = false;
		return;
	}
	ImGui::OpenPopup("Delete current scene?");
	LOG_INFO("POPUP INITIATED");
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("All those beautiful nodes will be deleted.\nThis operation cannot be undone!");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			App::getModule<StateManager>().newScene();
			popupActive = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			popupActive = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void StartWindow::renderTutorials() {}
