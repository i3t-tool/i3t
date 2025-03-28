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

#include <cmath>
#include <iostream>

#include "DIWNE/Core/NodeEditor.h"

namespace DIWNE
{
DiwnePanel::DiwnePanel(NodeEditor& editor, std::string label) : m_editor(editor), m_label(label) {}

void DiwnePanel::begin()
{
	reset();
	ImGui::PushID(m_label.c_str());
	DGui::BeginGroup();
}

void DiwnePanel::end(DiwnePanel* parent)
{
	ImGui::EndGroup();
	ImGui::PopID();

	m_lastScreenRect.Min = ImGui::GetItemRectMin(); // Last screen space rect is retained temporarily
	m_lastScreenRect.Max = ImGui::GetItemRectMax();
	m_rect = m_editor.canvas().screen2diwne(m_lastScreenRect);

	if (m_submittedSpringWidth <= 0.0f)
	{
		m_mode = FIXED; // Fixed mode
		m_submittedFixedWidth = m_rect.GetWidth();
	}
	else
	{
		m_mode = SPRINGY; // Springy mode
		// TODO: The assert below is probably too strong, perhaps a warning?
		if (m_submittedFixedWidth <= 0.0f)
		{
			DIWNE_WARN("[DiwnePanel] Panel only contains springs!")
			DIWNE_BREAKPOINT();
		}
		// The submitted fixed width cannot exceed the panels real width.
		m_submittedFixedWidth = std::min(m_submittedFixedWidth, m_rect.GetWidth());
	}

	// Submit itself to the parent panel
	if (parent != nullptr)
	{
		parent->submitChild(this);
	}

	DIWNE_DEBUG_LAYOUT((m_editor), {
		if (m_rect.GetArea() > 0.0f)
		{
			// TODO: Add display rect visualization
			m_editor.canvas().AddRectDiwne(m_rect.Min, m_rect.Max, ImColor(0, 0, 255, 100), 0,
			                               ImDrawFlags_RoundCornersNone, 1);
			ImVec2 originPos = ImVec2(m_rect.Min.x, m_rect.Max.y);
			ImGui::GetForegroundDrawList()->AddText(m_editor.canvas().diwne2screen(originPos) + ImVec2(0, 0),
			                                        IM_COL32_WHITE,
			                                        (std::string() + m_label)
			                                            //		     (m_ ? "Hovered\n" : "") + (m_isPressed ? "Held\n" :
			                                            //"") + (m_isDragged ? "Dragged\n" : "")
			                                            .c_str());
		}
	});
}

void DiwnePanel::layout()
{
	float newAvailSpringWidth = m_rect.GetWidth() - m_submittedFixedWidth;
	// Only change the spring width if the change is not insignificant, this is done to prevent oscillations due to
	// floating point precision issues
	// Diwne coordinates are essentially 1x zoom coordinates so it makes sense to use a delta of around a hundredth
	// A tiny change is still allowed but only to make springs smaller
	float newSpringDiff = newAvailSpringWidth - m_availableSpringWidth;
	if (!std::signbit(newSpringDiff) || fabsf(newSpringDiff) > 0.001f)
		m_availableSpringWidth = newAvailSpringWidth;

	m_availableSpringWidth = std::max(0.0f, m_availableSpringWidth);

	DIWNE_DEBUG_LAYOUT((m_editor), {
		if (m_rect.GetArea() > 0.0f)
		{
			m_editor.canvas().AddRectDiwne(m_rect.Min, m_rect.Max, ImColor(255, 0, 255, 100), 0,
			                               ImDrawFlags_RoundCornersNone, 2);
			ImVec2 originPos = ImVec2(m_rect.Min.x, m_rect.Max.y);
			ImGui::GetForegroundDrawList()->AddText(m_editor.canvas().diwne2screen(originPos) + ImVec2(0, 0),
			                                        IM_COL32_WHITE,
			                                        (std::string() + m_label)
			                                            //		     (m_ ? "Hovered\n" : "") + (m_isPressed ? "Held\n" :
			                                            //"") + (m_isDragged ? "Dragged\n" : "")
			                                            .c_str());
		}
	});
}

void DiwnePanel::submitItem()
{
	submitFixedWidth(m_editor.canvas().screen2diwneSize(ImGui::GetItemRectSize().x));
	applyQueuedWidth();
}

void DiwnePanel::submitItem(const std::shared_ptr<DiwneObject>& item)
{
	submitItem(item.get());
}

void DiwnePanel::submitItem(DiwneObject* item)
{
	if (!item->m_drawnThisFrame)
		return;
	submitFixedWidth(item->getRect().GetWidth());
	applyQueuedWidth();
}

void DiwnePanel::submitChild(DiwnePanel* child)
{
	submitFixedWidth(child->getMinimumWidth());
	applyQueuedWidth();
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

void DiwnePanel::reset()
{
	m_submittedFixedWidth = 0;
	m_submittedSpringWidth = 0;
	m_widthQueued = false;
	m_queuedFixedWidth = 0;
}

void DiwnePanel::spring(float relSize)
{
	// TODO: (DR) Add system to ensure that relSize for all springs doesn't exceed 1.0 per frame
	float springWidth = m_availableSpringWidth * relSize;
	if (springWidth > 0.0f)
	{
		ImGui::Dummy(ImVec2(m_editor.canvas().diwne2screenSize(springWidth), 0));
		// DIWNE_INFO(m_label +
		//            ": Created spring with size: " + std::to_string(m_editor.canvas().diwne2screenSize(springWidth)));
		ImGui::SameLine(0, 0);
		applyQueuedWidth();
		submitSpringWidth(springWidth);
	}
}

void DiwnePanel::sameLine(float spacing)
{
	ImGui::SameLine(0.0, spacing);
	queueFixedWidth(spacing == -1.0f ? m_editor.canvas().screen2diwneSize(ImGui::GetStyle().ItemSpacing.x) : spacing);
}

float DiwnePanel::setWidth(float width)
{
	assert(width >= 0); // Width needs to be positive (inversion not desirable)
	float prevWidth = getWidth();
	setMaxX(getMinX() + width);
	return width - prevWidth;
}

float DiwnePanel::getMinimumWidth() const
{
	if (m_mode == FIXED)
		return m_rect.GetWidth();
	if (m_mode == SPRINGY)
		return m_submittedFixedWidth;
	DIWNE_FAIL("Invalid DiwnePanel mode!");
	return 0;
}

} // namespace DIWNE