#include "BeforeNewModal.h"

#include <imgui.h>

#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputManager.h"
#include "State/StateManager.h"

void BeforeNewModal::onImGui()
{
	if (!App::getModule<StateManager>().isDirty())
	{
		NewProjectCommand::dispatch();
		hide();
	}

	ImGui::Text("Do you want to save your current project?\n\n");
	ImGui::Separator();

	if (ImGui::Button("Yes", ImVec2(100, 0)))
	{
		InputManager::triggerAction("save", EKeyState::Pressed);

		if (!App::getModule<StateManager>().isDirty())
		{
			// save was successful
			NewProjectCommand::dispatch();

			hide();
		}
	}
	ImGui::SetItemDefaultFocus();

	ImGui::SameLine();
	if (ImGui::Button("No", ImVec2(100, 0)))
	{
		NewProjectCommand::dispatch();

		hide();
	}

	ImGui::SameLine();
	if (ImGui::Button("Cancel", ImVec2(100, 0)))
	{
		hide();
	}
}
