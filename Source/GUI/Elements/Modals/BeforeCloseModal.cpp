#include "BeforeCloseModal.h"

#include <imgui.h>

#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputManager.h"
#include "State/StateManager.h"

void BeforeCloseModal::onImGui()
{
	if (!App::getModule<StateManager>().isDirty())
	{
		CloseCommand::dispatch();
		hide();
	}

	ImGui::Text("Any unsaved content will be discarded, are you sure?\n\n");
	ImGui::Separator();

	if (ImGui::Button("Save and quit", ImVec2(100, 0)))
	{
		InputManager::triggerAction("save", EKeyState::Pressed);

		if (!App::getModule<StateManager>().isDirty())
		{
			// save was successful
			CloseCommand::dispatch();

			hide();
		}
	}
	ImGui::SetItemDefaultFocus();

	ImGui::SameLine();
	if (ImGui::Button("Quit anyway", ImVec2(100, 0)))
	{
		CloseCommand::dispatch();

		hide();
	}

	ImGui::SameLine();
	if (ImGui::Button("Cancel", ImVec2(100, 0)))
	{
		hide();
	}
}
