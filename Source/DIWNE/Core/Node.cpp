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

#include "NodeEditor.h"
#include "diwne_actions.h"

// TODO: (DR) Remove these imports <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#include "GUI/Workspace/Nodes/Basic/CoreNode.h"
#include "GUI/Workspace/WorkspaceDiwne.h"

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
	return m_drawAnyway || getRectDiwne().Overlaps(diwne.canvas().getViewportRectDiwne()) || m_isDragged;
}

void Node::begin(DrawInfo& context)
{
	switch (m_nodePosMode)
	{
	case DrawModeNodePosition::OnItsPosition:
		ImGui::SetCursorScreenPos(diwne.canvas().diwne2screen(m_nodePositionDiwne));
		break;
	case DrawModeNodePosition::OnCursorPosition:
		setNodePositionDiwne(diwne.canvas().screen2diwne(ImGui::GetCursorScreenPos()));
		break;
	}
	if (m_drawAnyway)
		m_drawAnyway = false;

	ImGui::PushID(m_labelDiwne.c_str());
	ImGui::BeginGroup(); /* Begin of node */
}

void Node::content(DrawInfo& context)
{
	ImGui::Text("Node %d", m_idDiwne);
	m_rect.Min = ImGui::GetItemRectMin();
	m_rect.Max = ImGui::GetItemRectMax();
}

void Node::end(DrawInfo& context)
{
	DIWNE_DEBUG_OBJECTS((diwne), {
		ImRect rect = getRectDiwne();
		ImVec2 originPos = ImVec2(rect.Min.x, rect.Max.y);
		ImGui::GetForegroundDrawList()->AddText(
		    diwne.canvas().diwne2screen(originPos) + ImVec2(0, 0), m_destroy ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE,
		    fmt::format("D:{}-{}-{}-{}\nWA:{}-{}-{}-{}\nS:{}-{}-{}-{}", rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y,
		                diwne.canvas().diwne2workArea(rect.Min).x, diwne.canvas().diwne2workArea(rect.Min).y,
		                diwne.canvas().diwne2workArea(rect.Max).x, diwne.canvas().diwne2workArea(rect.Max).y,
		                diwne.canvas().diwne2screen(rect.Min).x, diwne.canvas().diwne2screen(rect.Min).y, diwne.canvas().diwne2screen(rect.Max).x,
		                diwne.canvas().diwne2screen(rect.Max).y)
		        .c_str());
	});
	ImGui::EndGroup();
	ImGui::PopID();
}

void Node::afterDrawDiwne(DrawInfo& context)
{
	// Adding an invisible ImGui blocking button to represent the logically opaque background of the node.
	// This needs to be done BEFORE processing interactions as we check if this button is hovered.
	ImGui::SetCursorScreenPos(diwne.canvas().diwne2screen(getNodePositionDiwne())); // TODO: Use m_rect instead
	ImGui::InvisibleButton("DiwneNodeBlockingButton", getRectDiwne().GetSize() * diwne.getZoom());
	m_internalHover = ImGui::IsItemHovered();
	DiwneObject::afterDrawDiwne(context);
}

bool Node::processSelectDiwne(DrawInfo& context)
{
	if (DiwneObject::processSelectDiwne(context))
		return true;

	// Check if the node is inside a selection rectangle
	if (auto action = context.state.getActiveAction<Actions::SelectionRectAction>())
	{
		bool inRect = action->touch ? action->rect.Overlaps(getRectDiwne()) : action->rect.Contains(getRectDiwne());
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

	// Draw node selection indicator
	if (m_selected)
	{
		diwne.canvas().AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max,
		                               diwne.mp_settingsDiwne->itemSelectedBorderColor,
		                               diwne.mp_settingsDiwne->selectionRounding, ImDrawFlags_RoundCornersAll,
		                               diwne.mp_settingsDiwne->itemSelectedBorderThicknessDiwne);
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
		std::vector<Node*> nodes;
		if (m_selected)
		{ // If the node is selected, it and all other selected nodes are dragged together
			// TODO: (DR) Avoid the new list as getSelectedNodes should just return a plain Node* pointer.
			for (auto&& node : diwne.getSelectedNodes())
				nodes.push_back(static_cast<Node*>(node.get()));
		}
		else
		{ // If the node isn't selected, dragging it selects it and deselects all others
			if (diwne.mp_settingsDiwne->selectNodeOnDrag)
			{
				diwne.deselectNodes();
				setSelected(true);
			}
			nodes.push_back(this);
		}
		context.state.startAction<Actions::DragNodeAction>(diwne, m_labelDiwne, std::move(nodes));
	}
	if (auto action = context.state.getActiveAction<Actions::DragNodeAction>(Actions::dragNode, m_labelDiwne))
	{
		action->onUpdate();
		if (dragEnd)
			action->end();
	}
}

void Node::onDestroy(bool logEvent)
{
	DiwneObject::onDestroy(logEvent);
	if (m_selected)
	{
		diwne.setNodesSelectionChanged(true);
	}

	auto lastActiveNode = diwne.getLastActiveNode<DIWNE::Node>();
	if (lastActiveNode != nullptr && lastActiveNode.get() == this)
	{
		diwne.setLastActiveNode<DIWNE::Node>(nullptr);
	}
}

} /* namespace DIWNE */
