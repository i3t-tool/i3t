#include "RemoveModelModal.h"

#include <imgui.h>

#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputManager.h"

RemoveModelModal::RemoveModelModal(std::string modelAlias, int usedCount, std::function<void()> onRemove)
    : m_modelAlias(modelAlias), m_usedCount(usedCount), m_onRemove(onRemove), ModalWindow("Remove model?")
{}

void RemoveModelModal::onImGui()
{
	ImGui::Text("The model '%s' is currently being used by %d model node%s.\n", m_modelAlias.c_str(), m_usedCount,
	            (m_usedCount > 1 ? "s" : ""));
	ImGui::Text("Are you sure you want to remove it?\n\n");

	ImGui::Separator();

	if (ImGui::Button("Yes", ImVec2(100, 0)))
	{
		m_onRemove();
		hide();
	}
	ImGui::SetItemDefaultFocus();

	ImGui::SameLine();
	if (ImGui::Button("No", ImVec2(100, 0)))
	{
		hide();
	}
}
