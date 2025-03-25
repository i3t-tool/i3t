/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "RemoveModelModal.h"

#include "imgui.h"

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

	if (ImGui::Button("Yes", ImVec2(4 * ImGui::GetFontSize(), 0)))
	{
		m_onRemove();
		hide();
	}
	ImGui::SetItemDefaultFocus();

	ImGui::SameLine();
	if (ImGui::Button("No", ImVec2(4 * ImGui::GetFontSize(), 0)))
	{
		hide();
	}
}
