//
// Created by Dan on 25.03.2025.
//

#include "VStack.h"

#include "DIWNE/Core/NodeEditor.h"

namespace DIWNE
{
void VStack::begin()
{
	assert(!m_buildingStack && "VStack::end() not called!");
	m_buildingStack = true;

	origin = ImGui::GetCursorScreenPos();
	m_currentRowIndex = 0;
	m_maxFixedWidth = 0;
}

void VStack::end()
{
	assert(m_buildingStack && "VStack::begin() not called!");
	m_buildingStack = false;

	// TODO: Probably somehow in an amortized way shrink the children vector if some rows were no longer reached.
	//   Which are still in use are ones with index < m_currentRowIndex

	// TODO: We have to relay VStack information to some parent
	//   If VStack was a DiwnePanel itself this would become easier I suppose.

	layout();
}

DiwnePanel* VStack::beginRow()
{
	assert(m_currentRowIndex >= 0);
	assert(!m_buildingRow && m_buildingStack && "VStack::begin() or VStack::endRow() not called!");
	m_buildingRow = true;
	DiwnePanel* panel;
	if (m_currentRowIndex >= m_children.size())
	{
		// Panel doesn't exist, the stack grows
		static const std::string label("VStack");
		m_children.emplace_back(m_editor, (label + std::to_string(m_currentRowIndex)).c_str());
		panel = &m_children.back();
	}
	else
	{
		// Panel already exists
		panel = &m_children.at(m_currentRowIndex);
	}

	// Position the new panel
	const DiwnePanel* lastPanel = m_currentRowIndex == 0 ? nullptr : &m_children[m_currentRowIndex - 1];
	if (!lastPanel)
	{
		ImGui::SetCursorScreenPos(origin);
	}
	else
	{
		// Using last panels screen rect as it should in this case correspond to the diwne rect (no layouting yet)
		ImGui::SetCursorScreenPos(
		    ImVec2(lastPanel->getLastScreenRectDiwne().Min.x, lastPanel->getLastScreenRectDiwne().Max.y));
	}

	panel->begin();
	return panel;
}

void VStack::endRow()
{
	assert(m_buildingRow && m_buildingStack && "VStack::begin() or VStack::beginRow() not called!");
	auto& panel = m_children.at(m_currentRowIndex);
	panel.end();
	m_maxFixedWidth = std::max(m_maxFixedWidth, panel.getMinimumWidth());

	m_buildingRow = false;
	m_currentRowIndex++;
}

void VStack::layout()
{
	// What is our goal?
	// We want to place several panels, so that:
	// They are placed AFTER each other vertically, with some gap (lets assume 0 gap for now)
	// They all have the same width, determined by the widest of them all.

	// Crucially, VStack is NOT a DiwnePanel on its own.
	// Why not? -> well it might be simpler for it to just be a manager of existing panels
	// Okay but those existing panels have a parent DiwnePanel -> yes
	// So this parent needs to know the real and minimum width -> yes
	// How?
	// Well, VStack is given a starting point, it moves the first panel there
	// Then it moves any other panel so that it starts at the right position below the first one
	// Then at the very end, it resizes all panels to some common width

	// And then, what, we can report this common width, as well as the minimum width
	// to the parent, okay, so we need to store it in the VStack object and then report it.
	// Instead, VStack might just be a DiwnePanel and do the storage and reporting itself
	// ... I tend to agree, VStack is an extension of a DiwnePanel, abstracting the insides to a unified API
	// Okay good idea, OR is it???
	// In the CoreNodeWithPins case, what do we want? Can m_right become a VStack -> NO
	// It can't, in the case of centering of the MULTIPLY pin, it would use another Panel to layout
	// So thats two potential "layouts" in a single Panel parent

	// Quick idea: What if VStack was actually a Layout object, and DiwnePanels could be assigned Layout objects
	// -> thats literally how swing works
	// so maybe thats not a bad idea.

	// Meaning m_right could be a DiwnePanel, with an assigned layout of VStack
	// ... Okay ... sure, how would we then solve the multiply pin centering?
	// Well, we would create another diwne panel
	// then what, add it as a child? Calculate real parent width from absolute positions?
	// We always need to retain min width though

	// Perhaps we will first implement pins and then deal with this centering nonsense

	// I like the idea of a "layouting" panel with a "layout", that contains (perhaps not statefully)
	// child panels that it manages and then keeps its sizing information up to date.

	// For start, lets keep VStack and Panel separate, maybe we join them later once I get the basic
	// logic down.

	float targetWidth = 0;
	for (auto& panel : m_children)
	{
		panel.setWidth(m_maxFixedWidth);
		panel.layout();
	}
}
} // namespace DIWNE