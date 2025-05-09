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
#include "SelectLayoutDialog.h"

#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "Core/Resources/ResourceManager.h"
#include "GUI/Fonts/Icons.h"
#include "GUI/I3TGui.h"
#include "GUI/WindowManager.h"


SelectLayoutDialog::SelectLayoutDialog() : IWindow(ICON_T(ICON_I3T_GRID " ", "Select Layout")) {}

void SelectLayoutDialog::render()
{

	ImVec2 center = ImGui::GetMainViewport()->GetWorkCenter();
	ImVec2 windowSize = ImVec2(ImGui::GetFontSize() * 25, 0);
	ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
	ImGui::Begin(getName(), getShowPtr(), g_dialogFlags | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);
	{
		this->updateWindowInfo();
		showSelectLayoutMenu();
	}
	ImGui::End();

	if (!isVisible())
	{
		HideWindowCommand::dispatch(ID);
	}
}

void SelectLayoutDialog::showSelectLayoutMenu()
{
	// clang-format off
	ImGui::TextWrapped(_tbd("Use this panel to manage window layouts."));
	ImGui::TextWrapped(_tbd("You can switch between existing layouts,") "\n"
						_tbd("create new ones to save your preferred window arrangement,") "\n"
						_tbd("or delete layouts you no longer need."));
	// clang-format on

	static std::set<std::string> userLayouts = getFilesInDirectory("Data/Layouts");
	static std::set<std::string> nonRemovableLayouts = getFilesInDirectory("Data/Layouts/NonRemovable");
	static std::string selectedFile;
	if (ImGui::BeginListBox("##LayoutsListbox"))
	{
		// Show non-removable userLayouts first
		for (auto& item : nonRemovableLayouts)
		{
			const bool isSelectedFile = (selectedFile == item);
			if (ImGui::Selectable(item.c_str(), isSelectedFile))
			{
				selectedFile = item;
				LoadWindowLayoutFromFileCommand::dispatch("Data/Layouts/NonRemovable/" + item + ".ini");
			}
			if (isSelectedFile)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		// Show removable userLayouts
		for (auto& item : userLayouts)
		{
			const bool isSelectedFile = (selectedFile == item);
			if (ImGui::Selectable(item.c_str(), isSelectedFile))
			{
				selectedFile = item;
				LoadWindowLayoutFromFileCommand::dispatch("Data/Layouts/" + item + ".ini");
			}
			if (isSelectedFile)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}


	if (userLayouts.find(selectedFile) != userLayouts.end()) // show delete button only for removable userLayouts
	{
		ImGui::SameLine();
		if (I3TGui::ButtonWithLog(_t("Delete")))
		{
			if (!selectedFile.empty() && userLayouts.find(selectedFile) != userLayouts.end())
			{
				deleteSelectedFile("Data/Layouts", selectedFile + ".ini");
				userLayouts.erase(selectedFile);
			}
		}
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	static char str1[128] = "";
	bool enterPressed = false;
	if (ImGui::InputTextWithHint("##input text", _t("new layout..."), str1, IM_ARRAYSIZE(str1),
	                             ImGuiInputTextFlags_EnterReturnsTrue))
	{
		enterPressed = true;
	}

	ImGui::SameLine();
	if ((I3TGui::ButtonWithLog(_t("Save")) || enterPressed) && str1[0] != '\0')
	{
		ImGui::SaveIniSettingsToDisk(("Data/Layouts/" + std::string(str1) + ".ini").c_str());
		userLayouts.insert(str1);
		selectedFile = str1;
		str1[0] = '\0';
	}
#ifdef I3T_DEBUG
	ImGui::Text("Debug only:");
	if ((I3TGui::ButtonWithLog("Save To NonRemovable ") || enterPressed) && str1[0] != '\0')
	{
		ImGui::SaveIniSettingsToDisk(("Data/Layouts/NonRemovable/" + std::string(str1) + ".ini").c_str());
		nonRemovableLayouts.insert(str1);
		selectedFile = str1;
		str1[0] = '\0';
	}
	if (I3TGui::ButtonWithLog("Delete From NonRemovable"))
	{
		if (!selectedFile.empty() && nonRemovableLayouts.find(selectedFile) != nonRemovableLayouts.end())
		{
			deleteSelectedFile("Data/Layouts/NonRemovable", selectedFile + ".ini");
			nonRemovableLayouts.erase(selectedFile);
		}
	}
#endif
}

std::set<std::string> SelectLayoutDialog::getFilesInDirectory(const std::string& path)
{
	std::set<std::string> fileNames;
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (entry.is_regular_file())
		{
			fileNames.insert(entry.path().stem().string());
		}
	}
	return fileNames;
}

void SelectLayoutDialog::deleteSelectedFile(const std::string& folderPath, const std::string& selectedFile)
{
	if (!selectedFile.empty())
	{
		fs::path filePath = fs::path(folderPath) / selectedFile;
		if (fs::remove(filePath))
		{
			LOG_INFO("Deleted: {}", filePath.string());
		}
		else
		{
			LOG_ERROR("Failed to delete: {}", filePath.string());
		}
	}
}
