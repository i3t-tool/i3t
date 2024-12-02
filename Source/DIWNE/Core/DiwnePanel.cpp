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

#include <iostream>

#include "NodeEditor.h"

namespace DIWNE
{
DiwnePanel::DiwnePanel(NodeEditor& editor, std::string label) : m_editor(editor), m_label(label) {}

void DiwnePanel::begin()
{
	reset();
	ImGui::PushID(m_label.c_str());
	ImGui::BeginGroup();
}

void DiwnePanel::end(DiwnePanel* parent)
{
	ImGui::EndGroup();
	ImGui::PopID();

	m_rect.Min = m_editor.screen2diwne(ImGui::GetItemRectMin());
	m_rect.Max = m_editor.screen2diwne(ImGui::GetItemRectMax());

	// Submit itself to the parent panel
	if (parent != nullptr)
	{
		parent->submitChild(this);
	}

	DIWNE_DEBUG_LAYOUT((m_editor), {
		if (m_rect.GetArea() > 0.0f)
		{
			m_editor.m_renderer->AddRectDiwne(m_rect.Min, m_rect.Max, ImColor(0, 0, 255, 100), 0,
			                                  ImDrawFlags_RoundCornersNone, 1);
			ImVec2 originPos = ImVec2(m_rect.Min.x, m_rect.Max.y);
			ImGui::GetForegroundDrawList()->AddText(
			    m_editor.diwne2screen(originPos) + ImVec2(0, 0), IM_COL32_WHITE,
			    (std::string() + m_label)
			        //		     (m_ ? "Hovered\n" : "") + (m_isHeld ? "Held\n" : "") + (m_isDragged ? "Dragged\n" : "")
			        .c_str());
		}
	});
}

void DiwnePanel::layout()
{
	m_availableSpringWidth = m_rect.GetWidth() - m_submittedFixedWidth;
//	if (m_availableSpringWidth < 0.0f)
//	{
//		std::cout << "[WARNING] DiwnePanel: Available spring width is negative! " << m_availableSpringWidth
//		          << std::endl;
//	}
	m_availableSpringWidth = std::max(0.0f, m_availableSpringWidth);
	DIWNE_DEBUG_LAYOUT((m_editor), {
		if (m_rect.GetArea() > 0.0f)
		{
			m_editor.m_renderer->AddRectDiwne(m_rect.Min, m_rect.Max, ImColor(255, 0, 255, 100), 0,
			                                  ImDrawFlags_RoundCornersNone, 2);
			ImVec2 originPos = ImVec2(m_rect.Min.x, m_rect.Max.y);
			ImGui::GetForegroundDrawList()->AddText(
			    m_editor.diwne2screen(originPos) + ImVec2(0, 0), IM_COL32_WHITE,
			    (std::string() + m_label)
			        //		     (m_ ? "Hovered\n" : "") + (m_isHeld ? "Held\n" : "") + (m_isDragged ? "Dragged\n" : "")
			        .c_str());
		}
	});
}

void DiwnePanel::reset()
{
	m_submittedFixedWidth = 0;
	m_submittedSpringWidth = 0;
	m_widthQueued = false;
	m_queuedFixedWidth = 0;
}

void DiwnePanel::submitFixedWidth(float width)
{
	assert(width >= 0);
	m_submittedFixedWidth += width;
}

void DiwnePanel::queueFixedWidth(float width)
{
	assert(width >= 0);
	if (width > 0.0f)
	{
		m_widthQueued = true;
		m_queuedFixedWidth += width;
	}
}

void DiwnePanel::submitSpringWidth(float width)
{
	assert(width >= 0);
	m_submittedSpringWidth += width;
}

void DiwnePanel::applyQueuedWidth()
{
	if (!m_widthQueued)
		return;
	submitFixedWidth(m_queuedFixedWidth);
	m_queuedFixedWidth = 0;
	m_widthQueued = false;
}

void DiwnePanel::spring(float relSize)
{
	float springWidth = m_availableSpringWidth * relSize;
	if (springWidth > 0.0f) {
		ImGui::Dummy(ImVec2(springWidth * m_editor.getWorkAreaZoom(), 0));
		ImGui::SameLine(0, 0);
		applyQueuedWidth();
		submitSpringWidth(springWidth);
	}
}

void DiwnePanel::sameLine(float spacing)
{
	ImGui::SameLine(0.0, spacing);
	queueFixedWidth(spacing == -1.0f ? ImGui::GetStyle().ItemSpacing.x / m_editor.getWorkAreaZoom() : spacing);
}

} // namespace DIWNE