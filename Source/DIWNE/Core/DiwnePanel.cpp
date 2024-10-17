/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \date 4.11.2024
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "DiwnePanel.h"

#include "NodeEditor.h"

namespace DIWNE
{
DiwnePanel::DiwnePanel(NodeEditor& editor, std::string label) : m_editor(editor), m_label(label) {}

void DiwnePanel::begin()
{
	ImGui::PushID(m_label.c_str());
	ImGui::BeginGroup();
}

void DiwnePanel::end()
{
	ImGui::EndGroup();
	ImGui::PopID();

	m_rect.Min = m_editor.screen2diwne(ImGui::GetItemRectMin());
	m_rect.Max = m_editor.screen2diwne(ImGui::GetItemRectMax());

	DIWNE_DEBUG_EXTRA_1((m_editor), {
		m_editor.m_renderer->AddRectDiwne(m_rect.Min, m_rect.Max, ImColor(0, 0, 255, 100), 0,
		                                  ImDrawFlags_RoundCornersNone, 2);
	});
}

} // namespace DIWNE