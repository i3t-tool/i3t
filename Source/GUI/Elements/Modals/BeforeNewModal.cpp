#include "BeforeNewModal.h"

#include <imgui.h>

#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputManager.h"
#include "State/StateManager.h"

constexpr const char* MODAL_LABEL = "Create new project?###%s";

void BeforeNewModal::render()
{
	if (!App::getModule<StateManager>().isDirty())
	{
		submit();

		return;
	}

	ImGui::OpenPopup(MODAL_LABEL);

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetWorkCenter();
	// center.x *= 0.5f;
	// center.y *= 0.5f;

	// ImVec2 parent_pos = ImGui::GetWindowPos();
	// ImVec2 parent_size = ImGui::GetWindowSize();
	// ImVec2 center(parent_pos.x + parent_size.x * 0.5f, parent_pos.y +
	// parent_size.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal(MODAL_LABEL, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Do you want to save current project?\n\n");
		ImGui::Separator();

		if (ImGui::Button("Yes", ImVec2(100, 0)))
		{
			InputManager::triggerAction("save", EKeyState::Pressed);

			if (!App::getModule<StateManager>().isDirty())
			{
				submit();

				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::SetItemDefaultFocus();

		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(100, 0)))
		{
			submit();

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void BeforeNewModal::submit()
{
	NewProjectCommand::dispatch();
	HideWindowCommand::dispatch(ID);
}
