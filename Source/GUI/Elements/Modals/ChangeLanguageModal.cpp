/**
 * \file
 * \brief
 * \author Matvei Korikov <korikmat@fel.cvut.cz>
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#include "ChangeLanguageModal.h"

#include "../../../../Dependencies/imgui/imgui.h"
#include "../../../Commands/ApplicationCommands.h"
#include "../../WindowManager.h"


#include "../../../Localization/Localization.h"
#include "../../../State/StateManager.h"
#include "../../Fonts/Icons.h"

ChangeLanguageModal::ChangeLanguageModal() : ModalWindow(ICON_T(ICON_I3T_LANG " ", "Change Language")) {}
void ChangeLanguageModal::onImGui()
{
	static auto availableLanguages = LOCALIZATION.getAvailableLanguages();
	static int selectedLanguage = LOCALIZATION.currentLanguageID;
	if (ImGui::BeginListBox("##LanguageListBox"))
	{
		for (const auto& language : availableLanguages)
		{
			bool isSelected = (selectedLanguage == language.id);
			if (ImGui::Selectable(language.name.c_str(), isSelected))
			{
				selectedLanguage = language.id;
			}
		}
		ImGui::EndListBox();
	}
	ImGui::TextWrapped(_t("You need to restart the application for all changes to take effect."));

	if (ImGui::Button(_t("Cancel")))
	{
		selectedLanguage = LOCALIZATION.currentLanguageID;
		close();
	}
	ImGui::SameLine();
	if (ImGui::Button(_t("OK")))
	{
		LOCALIZATION.loadLanguage(selectedLanguage);
		LOCALIZATION.currentLanguageID = selectedLanguage;
		Application::getModule<StateManager>().saveUserData();
		close();
	}
}