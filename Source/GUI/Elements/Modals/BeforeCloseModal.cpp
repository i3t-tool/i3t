#include "BeforeCloseModal.h"

#include <imgui.h>

#include "Commands/ApplicationCommands.h"
#include "State/StateManager.h"

void BeforeCloseModal::render()
{
	/// \todo This is quick hotfix.
	if (!StateManager::instance().isDirty())
	{
		CloseCommand::dispatch();
		return;
	}

	ImGui::OpenPopup("Close?###%s");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetWorkCenter();
	// center.x *= 0.5f;
	// center.y *= 0.5f;

	// ImVec2 parent_pos = ImGui::GetWindowPos();
	// ImVec2 parent_size = ImGui::GetWindowSize();
	// ImVec2 center(parent_pos.x + parent_size.x * 0.5f, parent_pos.y + parent_size.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Close?###%s", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Any unsaved content will be discarded, are you sure?\n\n");
		ImGui::Separator();

		if (ImGui::Button("Save and quit", ImVec2(100, 0)))
		{
			StateManager::instance().saveScene();

			InputManager::triggerAction("save", EKeyState::Pressed);

			HideWindowCommand::dispatch(ID);
			CloseCommand::dispatch();

			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();

		ImGui::SameLine();
		if (ImGui::Button("Quit anyway", ImVec2(100, 0)))
		{
			HideWindowCommand::dispatch(ID);
			CloseCommand::dispatch();

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(100, 0)))
		{
			HideWindowCommand::dispatch(ID);

			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
