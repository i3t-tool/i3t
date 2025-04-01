//
// Created by Dan on 25.03.2025.
//

#include "Stack.h"

#include "DIWNE/Core/NodeEditor.h"

namespace DIWNE
{
void Stack::begin()
{
	assert(!m_buildingStack && "Stack::end() not called!");
	m_buildingStack = true;

	origin = ImGui::GetCursorScreenPos();
	m_currentIndex = 0;
	m_maxFixedSize = 0;
}

void Stack::end()
{
	assert(m_buildingStack && "Stack::begin() not called!");
	m_buildingStack = false;

	// TODO: Probably somehow in an amortized way shrink the children vector if some rows were no longer reached.
	//   Which are still in use are ones with index < m_currentRowIndex

	// TODO: We have to relay Stack information to some parent
	//   If Stack was a DiwnePanel itself this would become easier I suppose.

	layout();
}

DiwnePanel* Stack::beginItem()
{
	assert(m_currentIndex >= 0);
	assert(!m_buildingItem && m_buildingStack && "Stack::begin() or Stack::endItem() not called!");
	m_buildingItem = true;
	DiwnePanel* panel;
	if (m_currentIndex >= m_children.size())
	{
		// Panel doesn't exist, the stack grows
		static const std::string label("Stack");
		m_children.emplace_back(m_editor, (label + std::to_string(m_currentIndex)).c_str());
		panel = &m_children.back();
	}
	else
	{
		// Panel already exists
		panel = &m_children.at(m_currentIndex);
	}

	// Position the new panel
	const DiwnePanel* lastPanel = m_currentIndex == 0 ? nullptr : &m_children[m_currentIndex - 1];
	if (!lastPanel)
	{
		ImGui::SetCursorScreenPos(origin);
	}
	else
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 spacing;
		if (m_spacing != -1)
		{
			float customSpacing = m_editor.canvas().diwne2screenSize(m_spacing);
			spacing = ImVec2(customSpacing, customSpacing);
		}
		else
		{
			spacing = style.ItemSpacing;
		}
		// Using last panels screen rect as it should in this case correspond to the diwne rect (no layouting yet)
		const ImRect& lastPanelRect = lastPanel->getLastScreenRectDiwne();
		const ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		if (m_mode == VERTICAL)
		{
			const float ypos = !m_springAdded ? lastPanelRect.Max.y + spacing.y : cursorPos.y;
			ImGui::SetCursorScreenPos(ImVec2(lastPanelRect.Min.x, ypos));
		}
		else
		{
			const float xpos = !m_springAdded ? lastPanelRect.Max.x + spacing.x : cursorPos.x;
			ImGui::SetCursorScreenPos(ImVec2(xpos, lastPanelRect.Min.y));
		}
	}

	panel->begin();
	return panel;
}

void Stack::endItem()
{
	assert(m_buildingItem && m_buildingStack && "Stack::begin() or Stack::beginItem() not called!");
	auto& panel = m_children.at(m_currentIndex);
	panel.end();
	m_maxFixedSize = std::max(m_maxFixedSize, m_mode == VERTICAL ? panel.getMinimumWidth() : panel.getMinimumHeight());

	m_buildingItem = false;
	m_currentIndex++;
}

bool Stack::spring(float relSize)
{
	assert(m_panel && "The stack needs to have an enclosing DiwnePanel specified to use springs!");
	assert(m_buildingStack && !m_buildingItem && "Stack springs can only be added between items!");
	if (!m_panel)
		return false;
	bool springCreated;
	if (m_mode == VERTICAL)
		springCreated = m_panel->vspring(relSize);
	else
		springCreated = m_panel->spring(relSize);
	if (springCreated)
		m_springAdded = true; // Make next layout item begin at the end of the spring
	return springCreated;
}

void Stack::layout()
{
	for (auto& panel : m_children)
	{
		if (m_mode == VERTICAL)
			panel.setWidth(m_maxFixedSize);
		else
			panel.setHeight(m_maxFixedSize);
		panel.layout();
	}
}
} // namespace DIWNE