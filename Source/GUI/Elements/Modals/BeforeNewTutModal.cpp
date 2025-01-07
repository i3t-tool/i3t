/**
 * \file
 * \brief
 * \author Adam Loucký
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "BeforeNewTutModal.h"

#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputManager.h"
#include "GUI/Elements/Windows/StartWindow.h"
#include "I3T.h"
#include "State/StateManager.h"
#include "Tutorial/TutorialManager.h"

void BeforeNewTutModal::onImGui()
{
	if (!App::getModule<StateManager>().isDirty())
	{
		submit();
	}

	ImGui::Text("Save the current scene?\n\n");
	ImGui::Separator();

	if (ImGui::Button("Yes", ImVec2(100, 0)))
	{
		InputManager::triggerAction("save", EKeyState::Pressed);

		if (!App::getModule<StateManager>().isDirty())
		{
			submit();
		}
	}
	ImGui::SetItemDefaultFocus();

	ImGui::SameLine();
	if (ImGui::Button("No", ImVec2(100, 0)))
	{
		submit();
	}
	ImGui::SameLine();

	if (ImGui::Button("Cancel", ImVec2(100, 0)))
	{
		hide();
		*I3T::getWindowPtr<StartWindow>()->getShowPtr() = true;
	}
	ImGui::SameLine();
}

void BeforeNewTutModal::submit()
{
	auto& windowManager = I3T::getUI()->getWindowManager();

	Ptr<Tutorial> tut = TutorialManager::instance().getTutorial();
	if (!App::getModule<StateManager>().loadScene(tut->m_header->m_scene))
	{
		// Tutorial has no scene, create new one.
		NewProjectCommand::dispatch();
	}
	hide();
	SetTutorialCommand::dispatch(tut);
	Ptr<IWindow> tutorialWindow = I3T::getWindowPtr<TutorialWindow>();
	windowManager.showWindow(tutorialWindow, true);
	windowManager.focusWindow(tutorialWindow);

	// load layout for the tutorial if it is defined
	if (tut->m_header->m_layout != "undefined")
	{
		LoadWindowLayoutFromFileCommand::dispatch(tut->m_header->m_layout);
	}
}
