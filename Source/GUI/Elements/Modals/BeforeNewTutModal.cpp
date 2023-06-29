#include "BeforeNewTutModal.h"

#include <imgui.h>

#include "Commands/ApplicationCommands.h"
#include "Core/API.h"
#include "Core/Input/InputManager.h"
#include "GUI/Elements/Windows/StartWindow.h"
#include "State/StateManager.h"

constexpr const char* MODAL_LABEL = "Opening a new tutorial###%s";

void BeforeNewTutModal::render()
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
		ImGui::Text("Save the current scene?\n\n");
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
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(100, 0)))
		{
			HideWindowCommand::dispatch(ID);
			*I3T::getWindowPtr<StartWindow>()->getShowPtr() = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();

		ImGui::EndPopup();
	}
}

void BeforeNewTutModal::submit()
{
	Ptr<Tutorial> tut = I3T::getUI()->getWindowManager().getWindowPtr<StartWindow>()->getTutorial();
	if (!App::getModule<StateManager>().loadScene(tut->m_header->m_scene))
	{
		NewProjectCommand::dispatch();
	}
	HideWindowCommand::dispatch(ID);
	SetTutorialCommand::dispatch(tut);
	Ptr<IWindow> tutorialWindow = I3T::getWindowPtr<TutorialWindow>();
	I3T::getUI()->getWindowManager().showWindow(tutorialWindow, true);
	I3T::getUI()->getWindowManager().focusWindow(tutorialWindow);
}
