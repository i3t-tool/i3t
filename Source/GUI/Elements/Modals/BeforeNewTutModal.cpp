#include "BeforeNewTutModal.h"

#include <imgui.h>

#include "API.h"
#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputManager.h"
#include "GUI/Elements/Windows/StartWindow.h"
#include "State/StateManager.h"

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

	Ptr<Tutorial> tut = windowManager.getWindowPtr<StartWindow>()->getTutorial();
	if (!App::getModule<StateManager>().loadScene(tut->m_header->m_scene))
	{
		NewProjectCommand::dispatch();
	}
	hide();
	SetTutorialCommand::dispatch(tut);
	Ptr<IWindow> tutorialWindow = I3T::getWindowPtr<TutorialWindow>();
	windowManager.showWindow(tutorialWindow, true);
	windowManager.focusWindow(tutorialWindow);
}
