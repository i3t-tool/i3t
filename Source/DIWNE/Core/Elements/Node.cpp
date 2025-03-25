/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Node.h"

#include "Logger/Logger.h"

#include "DIWNE/Core/NodeEditor.h"
#include "DIWNE/Core/diwne_actions.h"

namespace DIWNE
{
Node::Node(DIWNE::NodeEditor& diwne, std::string labelDiwne) : DiwneObject(diwne, labelDiwne) {}

Node::~Node() {}

Node& Node::operator=(const Node& rhs)
{
	if (this == &rhs)
		return *this; // handle self assignment
	// assignment operator
	return *this;
}

bool Node::allowDrawing()
{
	ImRect viewportRect = diwne.canvas().getViewportRectDiwne();
	return DiwneObject::allowDrawing() || getRect().Overlaps(viewportRect) || m_isDragged;
}

void Node::begin(DrawInfo& context)
{
	ImGui::PushID(m_labelDiwne.c_str());
	ImGui::BeginGroup(); /* Begin of node */
}

void Node::content(DrawInfo& context)
{
	ImGui::Text("Node %d", m_idDiwne);
}

void Node::end(DrawInfo& context)
{
	DIWNE_DEBUG_OBJECTS((diwne), {
		ImRect rect = getRect();
		ImVec2 originPos = ImVec2(rect.Min.x, rect.Max.y);
		ImGui::GetForegroundDrawList()->AddText(
		    diwne.canvas().diwne2screen(originPos) + ImVec2(0, 0),
		    m_destroy ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE,
		    fmt::format("D:{}-{}-{}-{}\nWA:{}-{}-{}-{}\nS:{}-{}-{}-{}", rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y,
		                diwne.canvas().diwne2workArea(rect.Min).x, diwne.canvas().diwne2workArea(rect.Min).y,
		                diwne.canvas().diwne2workArea(rect.Max).x, diwne.canvas().diwne2workArea(rect.Max).y,
		                diwne.canvas().diwne2screen(rect.Min).x, diwne.canvas().diwne2screen(rect.Min).y,
		                diwne.canvas().diwne2screen(rect.Max).x, diwne.canvas().diwne2screen(rect.Max).y)
		        .c_str());
	});
	ImGui::EndGroup();
	// Node ImGui ID gets popped later in afterDrawDiwne, see explanation there.
}

void Node::updateLayout(DrawInfo& context)
{
	m_rect.Min = diwne.canvas().screen2diwne(ImGui::GetItemRectMin());
	m_rect.Max = diwne.canvas().screen2diwne(ImGui::GetItemRectMax());
}

void Node::afterDrawDiwne(DrawInfo& context)
{
	// Adding an invisible ImGui blocking button to represent the logically opaque background of the node.
	// This is done after updateLayout() in order to know the current node size (otherwise it would be done in end())
	// This needs to be done BEFORE processing interactions as we determine if the node is hovered using it.
	// The blocking button uses the NoHoldingActiveId flag in order to avoid setting active id when dragging the node.
	ImGui::SetCursorScreenPos(diwne.canvas().diwne2screen(getPosition()));
	ImGui::InvisibleButton("DiwneNodeBlockingButton", getRect().GetSize() * diwne.getZoom(),
	                       ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_NoHoldingActiveId);
	m_internalHover = ImGui::IsItemHovered();
	ImGui::PopID();

	drawSelectionIndicator(context);

	DiwneObject::afterDrawDiwne(context);
}

bool Node::processSelectDiwne(DrawInfo& context)
{
	if (DiwneObject::processSelectDiwne(context))
		return true;

	// Check if the node is inside a selection rectangle
	if (auto action = context.state.getActiveAction<Actions::SelectionRectAction>())
	{
		bool inRect = action->touch ? action->rect.Overlaps(getRect()) : action->rect.Contains(getRect());
		bool multiSelect = diwne.input().multiSelectionActive();
		bool multiDeselect = diwne.input().multiDeselectionActive();
		if (inRect)
		{
			setSelected(multiDeselect ? false : true);
		}
		else
		{
			if (!multiSelect && !multiDeselect)
				setSelected(false);
		}
	}
	return false;
}

void Node::onSelection(bool selected)
{
	DiwneObject::onSelection(selected);
	diwne.setNodesSelectionChanged(true);
}

void Node::onDrag(DrawInfo& context, bool dragStart, bool dragEnd)
{
	DiwneObject::onDrag(context, dragStart, dragEnd);

	// Handle node dragging
	if (dragStart)
	{
		std::vector<std::shared_ptr<Node>> nodes;
		if (m_selected)
		{
			// When dragging a selected node we only drag other selected nodes on the same level
			// Eg. those that are in the same container == those that have the same parent
			DiwneObject* ourParent = this->getParentObject();
			auto selectedNodes = diwne.getSelectedNodesInnerIncluded();
			for (auto node = selectedNodes.begin(); node != selectedNodes.end(); ++node)
			{
				if (node->getParentObject() != ourParent)
					continue;
				if (node->isFixed() && node.ptr() != this) // Fixed nodes can be only dragged directly
					continue;
				nodes.push_back(node.sharedPtr());
				node->setBringToFront(true);
			}
		}
		else
		{
			// If the node isn't selected, dragging it selects it and deselects all others
			// or only the node is dragged
			if (diwne.mp_settingsDiwne->selectNodeOnDrag)
			{
				diwne.deselectAllNodes();
				setSelected(true);
			}
			nodes.push_back(this->sharedPtr<Node>());
			this->setBringToFront(true);
		}
		context.state.startAction<Actions::DragNodeAction>(diwne, shared_from_this(), std::move(nodes));
	}
	if (auto action = context.state.getActiveAction<Actions::DragNodeAction>(Actions::dragNode, this))
	{
		if (dragEnd)
		{
			action->end();
			diwne.m_takeSnap = true;
		}
	}
}

void Node::onDestroy(bool logEvent)
{
	if (m_selected)
	{
		diwne.setNodesSelectionChanged(true);
	}
	diwne.m_takeSnap = true;
	DiwneObject::onDestroy(logEvent);
}

bool Node::getFlag(char index) const
{
	return m_flag & (1 << index);
}

void Node::setFlag(char index, bool value)
{
	if (value)
		m_flag |= 1 << index;
	else
		m_flag &= ~(1 << index);
}

void Node::drawSelectionIndicator(DrawInfo& context)
{
	if (m_selected)
	{
		diwne.canvas().AddRectDiwne(getRect().Min, getRect().Max, diwne.mp_settingsDiwne->itemSelectedBorderColor,
		                            diwne.mp_settingsDiwne->selectionRounding, ImDrawFlags_RoundCornersAll,
		                            diwne.mp_settingsDiwne->itemSelectedBorderThicknessDiwne);
	}
}
bool Node::willBeRemovedFromContainer(const DiwneObject* container)
{
	return getParentObject() != container;
}
} /* namespace DIWNE */