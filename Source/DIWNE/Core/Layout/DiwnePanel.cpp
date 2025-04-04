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

#include "Logger/Logger.h" //TODO: Remove

#include "DIWNE/Core/NodeEditor.h"

namespace DIWNE
{
DiwnePanel::DiwnePanel(NodeEditor& editor, std::string label) : m_editor(editor), m_label(label) {}

void DiwnePanel::begin()
{
	reset();
	ImGui::PushID(m_label.c_str());
	DGui::BeginGroup();

	beginFixedItem();
}

void DiwnePanel::end(DiwnePanel* parent)
{
	endFixedItem(); // Must be before EndGroup()

	ImGui::EndGroup();
	ImGui::PopID();

	m_lastScreenRect.Min = ImGui::GetItemRectMin(); // Last screen space rect is retained temporarily
	m_lastScreenRect.Max = ImGui::GetItemRectMax();
	m_rect = m_editor.canvas().screen2diwne(m_lastScreenRect);

	// Submit itself to the parent panel
	if (parent != nullptr)
	{
		parent->submitChild(this);
	}

	DIWNE_DEBUG_LAYOUT((m_editor), {
		if (m_rect.GetArea() > 0.0f)
		{
			ImRect screenRect = m_editor.canvas().diwne2screen(m_rect);
			ImVec2 originPos = ImVec2(m_rect.Min.x, m_rect.Max.y);
			if (ImGui::IsMouseHoveringRect(screenRect.Min, screenRect.Max))
			{
				m_editor.canvas().AddRectForegroundDiwne(m_rect.Min + ImVec2(1, 1), m_rect.Max - ImVec2(1, 1),
				                                         ImColor(0, 204, 255, 255), 0, ImDrawFlags_RoundCornersNone, 1,
				                                         true);
			}
			else
			{
				m_editor.canvas().AddRectDiwne(m_rect.Min + ImVec2(1, 1), m_rect.Max - ImVec2(1, 1),
				                               ImColor(0, 204, 255, 150), 0, ImDrawFlags_RoundCornersNone, 1, true);
			}
		}
	});
}

void DiwnePanel::layout()
{
	ImVec2 newAvailSpringSize = m_rect.GetSize() - m_fixedSize;

	newAvailSpringSize = ImMax(ImVec2(), newAvailSpringSize);
	if (newAvailSpringSize.x < DIWNE_PIXEL_EPSILON)
		newAvailSpringSize.x = 0.0f;
	if (newAvailSpringSize.y < DIWNE_PIXEL_EPSILON)
		newAvailSpringSize.y = 0.0f;

	// Only change the spring size if the change is not insignificant, this is done to prevent oscillations due to
	// floating point precision issues
	// Diwne coordinates are essentially 1x zoom coordinates so it makes sense to use a delta of around a hundredth
	// A tiny change is still allowed but only to make springs smaller
	ImVec2 newSpringDiff = newAvailSpringSize - m_availableSpringSize;
	if (!std::signbit(newSpringDiff.x) || fabsf(newSpringDiff.x) > DIWNE_PIXEL_EPSILON)
		m_availableSpringSize.x = newAvailSpringSize.x;
	if (!std::signbit(newSpringDiff.y) || fabsf(newSpringDiff.y) > DIWNE_PIXEL_EPSILON)
		m_availableSpringSize.y = newAvailSpringSize.y;

	DIWNE_DEBUG_LAYOUT((m_editor), {
		if (m_rect.GetArea() > 0.0f)
		{
			ImRect screenRect = m_editor.canvas().diwne2screen(m_rect);
			ImVec2 originPos = ImVec2(m_rect.Min.x, m_rect.Max.y);
			if (ImGui::IsMouseHoveringRect(screenRect.Min, screenRect.Max))
			{
				m_editor.canvas().AddRectForegroundDiwne(m_rect.Min, m_rect.Max, ImColor(255, 0, 255, 255), 0,
				                                         ImDrawFlags_RoundCornersNone, 1, true);
				ImGui::GetForegroundDrawList()->AddText(
				    m_editor.canvas().diwne2screen(originPos) + ImVec2(0, 0), IM_COL32(255, 255, 255, 150),
				    (std::string() + m_label + "\nRect size: " + std::to_string(m_rect.GetSize().x) + ", " +
				     std::to_string(m_rect.GetSize().y) + "\nFixed size: " + std::to_string(m_fixedSize.x) + ", " +
				     std::to_string(m_fixedSize.y) + "\nAvail spring: " + std::to_string(m_availableSpringSize.x) +
				     ", " + std::to_string(m_availableSpringSize.y))
				        .c_str());
			}
			else
			{
				m_editor.canvas().AddRectDiwne(m_rect.Min, m_rect.Max, ImColor(255, 0, 255, 150), 0,
				                               ImDrawFlags_RoundCornersNone, 1, true);
			}
		}
	});
}

void DiwnePanel::submitChild(DiwnePanel* child)
{
	// TODO: Some special logic to forward only portion of the fixed with
	// In principle we DON'T want to subtract spring width as that might oscillate again
	// submitFixedWidth(child->getMinimumWidth());
	// applyQueuedWidth();
}

void DiwnePanel::expectWidthChangeThisFrame(float width)
{
	m_availableSpringSize.x -= width;
}

void DiwnePanel::reset()
{
	m_fixedSize = ImVec2();
	hsprings = vsprings = 0;
}
void DiwnePanel::beginFixedItem()
{
	m_lastFixedPositionXY = ImGui::GetCursorScreenPos();
}
void DiwnePanel::endFixedItem()
{
	// TODO: Not the whole size has to be fixed here if a DiwnePanel was submitted inside
	//   Presumably in DiwnePanel::end(parent) we submit the actual fixed size that panel had
	//   I'm not sure how that would work if other ImGui items got mixed into this though
	// TODO: It is expected that horizontal springs don't overlay any other fixed elements
	//   Meaning it isn't expected that there is anything above or below the spring
	//   In other words it is expected that all content is on a single line
	//   This is not necessarily a requirement, just the current use case.
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	// ImVec2 newCursorPos =
	//     ImVec2(window->DC.CursorPosPrevLine.x, window->DC.CursorPos.y - ImGui::GetStyle().ItemSpacing.y);

	ImVec2 newCursorPos = window->DC.CursorMaxPos;
	if (window->DC.IsSameLine) // Can't ignore SameLine spacing here
		newCursorPos = ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);

	// TODO: Perhaps when SameLine is detected prior to this call its width could be queued? Like in the old impl.
	ImVec2 newFixedPositionXY = ImMax(newCursorPos, m_lastFixedPositionXY);
	ImVec2 newFixedSize = newFixedPositionXY - m_lastFixedPositionXY;
	m_fixedSize += m_editor.canvas().screen2diwneSize(newFixedSize);
	if (!window->DC.IsSameLine) // We don't want vertical spacing before springs or after the panel
		window->DC.CursorPos = ImVec2(window->DC.CursorPos.x, newFixedPositionXY.y);
}

bool DiwnePanel::vspring(float relSize)
{
	return spring(relSize, false);
}

bool DiwnePanel::spring(float relSize, bool horizontal)
{
	ImGuiWindow* window = ImGui::GetCurrentWindowRead();

	// TODO: (DR) Add system to ensure that relSize for all springs doesn't exceed 1.0 per fram
	ImVec2 springSize = m_availableSpringSize * relSize;
	if ((horizontal && springSize.x <= 0.0f) || (!horizontal && springSize.y <= 0.0f))
		return false;

	if (horizontal)
		hsprings++;
	else
		vsprings++;

	endFixedItem();

	// Current line cursor should advance by the amount we're submitting as spring size (nearly exactly due to convert)
#if DIWNE_DEBUG_ENABLED
	ImVec2 sanityCursorPos1 = window->DC.CursorPos;
#endif

	ImVec2 springScreenSize;
	if (horizontal)
	{
		springScreenSize = ImVec2(m_editor.canvas().diwne2screenSize(springSize.x), 0);
		ImGui::Dummy(springScreenSize);
		ImGui::SameLine(0, 0);
	}
	else
	{
		// Vertical springs must be truncated to the nearest pixel
		springScreenSize = ImVec2(0, DIWNE_TRUNC(m_editor.canvas().diwne2screenSize(springSize.y)));
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 spacingBackup = style.ItemSpacing;
		style.ItemSpacing = ImVec2(0, 0); // We don't want any vertical spacing after the spring
		ImGui::Dummy(springScreenSize);
		style.ItemSpacing = spacingBackup;
	}

	DIWNE_DEBUG_LAYOUT(m_editor, { DDraw::drawSpringDebug(sanityCursorPos1, springScreenSize, horizontal); });
#if DIWNE_DEBUG_ENABLED
	ImVec2 sanityCursorPos2 = horizontal ? window->DC.CursorPosPrevLine : window->DC.CursorPos;
	ImVec2 diff = sanityCursorPos2 - sanityCursorPos1;
	float diffF = horizontal ? diff.x : diff.y;
	float springSizeF =
	    horizontal ? springScreenSize.x : DIWNE_TRUNC(sanityCursorPos1.y + springScreenSize.y) - sanityCursorPos1.y;
	bool test = DMath::equals(springSizeF, diffF, DIWNE_PIXEL_EPSILON);
	if (!test)
	{
		DIWNE_LOG_DEBUG(std::string("[DiwnePanel] Submitted ") + (horizontal ? "horizontal" : "vertical") +
		                " spring size doesn't correspond to the created ImGui item!\n" +
		                "Spring size: " + std::to_string(springSizeF) + "\nReal size: " + std::to_string(diffF))
	}
	assert(test);
#endif

	DIWNE_TRUNC_POS();
	beginFixedItem();
	return true;
}

void DiwnePanel::setWidth(float width)
{
	assert(width >= 0);
	setMaxX(getMinX() + width);
}

void DiwnePanel::setHeight(float height)
{
	assert(height >= 0);
	setMaxY(getMinY() + height);
}

float DiwnePanel::getMinimumWidth() const
{
	return m_fixedSize.x;
}

float DiwnePanel::getMinimumHeight() const
{
	return m_fixedSize.y;
}
ImVec2 DiwnePanel::getMinimumSize() const
{
	return m_fixedSize;
}
float DiwnePanel::getMinMaxX() const
{
	return m_rect.Min.x + m_fixedSize.x;
}
float DiwnePanel::getMinMaxY() const
{
	return m_rect.Min.y + m_fixedSize.y;
}

} // namespace DIWNE