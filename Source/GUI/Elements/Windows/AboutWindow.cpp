/**
 * \file
 * \brief
 * \authors Martin Herich <martin.herich@phire.cz>, Petr Felkel <felkel@fel.cvut.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "AboutWindow.h"

#include "imgui.h"

#include "I3T.h"

#include "Config.h"
#include "GUI/Elements/Windows/StartWindow.h"
#include "Utils/System.h"

static const char* I3T_WEBSITE = "https://i3t-tool.org/";

AboutWindow::AboutWindow() : IWindow("About") {}

void AboutWindow::render()
{
	// Set initial window size
	ImVec2 windowSize = ImVec2(1100, 735);
	static bool firstTime = true;
	if (firstTime)
	{
		firstTime = false;
		ImGui::SetNextWindowSize(windowSize);
		ImGui::SetNextWindowPos(ImVec2(1000.0f, 500.0f), 0, ImVec2(0.5f, 0.5f));
	}

	ImGui::PushStyleColor(ImGuiCol_WindowBg, I3T::getUI()->getTheme().get(EColor::AboutWindow_BackgroundLeft));
	ImGui::Begin(getName(), getShowPtr(),
	             ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
	                 ImGuiWindowFlags_AlwaysUseWindowPadding);
	{
		this->updateWindowInfo();
		I3T::getWindowPtr<StartWindow>()->renderLeftPanel();

		ImGui::SameLine();


		// ImGui::Columns(2, NULL, false);

		// ImGui::Image((void*)(intptr_t)dcgiReg->getTexId(), ImVec2(243.0f, 66.0f),
		// ImVec2(0, 1), ImVec2(1, 0));

		// ImGui::NextColumn();

		// right panel
		ImGui::PushStyleColor(ImGuiCol_ChildBg, I3T::getUI()->getTheme().get(EColor::AboutWindow_BackgroundRight));
		ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::AboutWindow_Text));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
		ImGui::BeginChild("panelAbout", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
		{
			ImGui::BeginGroup();
			{

				ImGui::Dummy(ImVec2(0, 10));

				ImGui::PushFont(I3T::getFont(EFont::TutorialTitle));
				ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialTitleText));
				ImGui::SetWindowFontScale(0.75f);
				ImGui::Text("About");
				ImGui::SetWindowFontScale(1.0f);
				ImGui::PopStyleColor();
				ImGui::PopFont();

				ImGui::TextWrapped("The I3T, The Interactive Tool for Teaching Transformations "
				                   "is the result of the work of students at three Parts of the Czech Technical "
				                   "University in Prague:\n");
				ImGui::Dummy(ImVec2(0, 5));
				ImGui::BulletText(
				    "The Department of Computer Graphics and Interaction of the Faculty of Electrical Engineering,");
				ImGui::BulletText("The Department of Computer Science of the Faculty of Electrical Engineering,");
				ImGui::BulletText("The Faculty of Information Technology.");
				ImGui::TextWrapped("\nThe first version was created by Michal Folta in 2018.\n\n");

				ImGui::PushFont(I3T::getFont(EFont::TutorialTitle));
				ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialTitleText));
				ImGui::SetWindowFontScale(0.75f);
				ImGui::Text("I3T Website");
				ImGui::SetWindowFontScale(1.0f);
				ImGui::PopFont();
				ImGui::PopStyleColor();

				ImGui::PushStyleColor(ImGuiCol_Button,
				                      I3T::getUI()->getTheme().get(EColor::AboutWindow_BackgroundRight));
				if (ImGui::Button(I3T_WEBSITE))
				{
					systemOpen(I3T_WEBSITE);
				}
				ImGui::PopStyleColor();

				ImGui::Text("");

				ImGui::PushFont(I3T::getFont(EFont::TutorialTitle));
				ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialTitleText));
				ImGui::SetWindowFontScale(0.75f);
				ImGui::Text("Supervisor");
				ImGui::SetWindowFontScale(1.0f);
				ImGui::PopStyleColor();
				ImGui::PopFont();

				ImGui::Text(Config::SUPERVISOR.c_str());
				ImGui::Text(Config::SUPERVISOR_MAIL.c_str());
				ImGui::Text("");

				ImGui::PushFont(I3T::getFont(EFont::TutorialTitle));
				ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialTitleText));
				ImGui::SetWindowFontScale(0.75f);
				ImGui::Text("Authors");
				ImGui::SetWindowFontScale(1.0f);
				ImGui::PopStyleColor();
				ImGui::PopFont();

				ImGui::Text(Config::AUTHOR.c_str());
				ImGui::Text("Martin Herich");
				ImGui::Text("Jaroslav Holeček");
				ImGui::Text("Dan Rakušan");
				ImGui::Text("Adam Loucký");
				ImGui::Text("Miroslav Mueller");
				ImGui::Text("Vít Zadina");
				ImGui::Text("Filip Uhlík");
				// ImGui::Text("Marek Nechanský"); // after addition of the Automatic box placement
				ImGui::Text("");


				ImGui::PushFont(I3T::getFont(EFont::TutorialTitle));
				ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialTitleText));
				ImGui::SetWindowFontScale(0.75f);
				ImGui::Text("Design");
				ImGui::SetWindowFontScale(1.0f);
				ImGui::PopStyleColor();
				ImGui::PopFont();

				ImGui::Text("Lukáš Pilka, 2018");
				ImGui::Text("Jaroslav Kolář, 2023");
				ImGui::Text("");

				ImGui::PushFont(I3T::getFont(EFont::TutorialTitle));
				ImGui::PushStyleColor(ImGuiCol_Text, I3T::getUI()->getTheme().get(EColor::TutorialTitleText));
				ImGui::SetWindowFontScale(0.75f);
				ImGui::Text("License");
				ImGui::SetWindowFontScale(1.0f);
				ImGui::PopStyleColor();
				ImGui::PopFont();

				ImGui::Text("Copyright (C) 2023. Open source under GNU GPL v3.0 License "
				            "(for details see the attached file LICENSE.txt)");

				ImGui::EndGroup();
			}

			ImGui::EndChild(); // right panel
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleColor(2);


		// ImGui::Separator();

		// ImGui::BeginHorizontal("AboutFooter",
		// ImVec2{ImGui::GetWindowContentRegionMax().x, 0.0f});
		// ImGui::Text(Config::VERSION.c_str());
		// if (ImGui::Button("OK", ImVec2{100.0f, 0.0f}))
		// {
		//	 HideWindowCommand::dispatch(ID);
		// }
		// ImGui::EndHorizontal();
	}
	ImGui::PopStyleColor();
	ImGui::End();
}
