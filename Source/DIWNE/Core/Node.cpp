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

void Node::deleteActionDiwne()
{
	if (m_toDelete) // Only delete once
		return;

	deleteAction();

	if (m_selected)
	{
		diwne.setNodesSelectionChanged(true);
	}

	auto lastActiveNode = diwne.getLastActiveNode<DIWNE::Node>();
	if (lastActiveNode != nullptr && lastActiveNode.get() == this)
	{
		diwne.setLastActiveNode<DIWNE::Node>(nullptr);
		diwne.setLastActivePin<DIWNE::Pin>(nullptr);
	}
	m_toDelete = true;
}

bool Node::allowDrawing()
{
	return m_drawAnyway || getRectDiwne().Overlaps(diwne.getWorkAreaDiwne()) || m_isDragged;
}

void Node::begin(DrawInfo& context)
{
	switch (m_nodePosMode)
	{
	case DrawModeNodePosition::OnItsPosition:
		ImGui::SetCursorScreenPos(diwne.diwne2screen(m_nodePositionDiwne));
		break;
	case DrawModeNodePosition::OnCursorPosition:
		setNodePositionDiwne(diwne.screen2diwne(ImGui::GetCursorScreenPos()));
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
	DIWNE_DEBUG_EXTRA_2((diwne), {
		ImRect nodeRectDiwne = getRectDiwne();
		ImGui::Text(fmt::format("D:{}-{}-{}-{}\nWA:{}-{}-{}-{}\nS:{}-{}-{}-{}", nodeRectDiwne.Min.x,
		                        nodeRectDiwne.Min.y, nodeRectDiwne.Max.x, nodeRectDiwne.Max.y,
		                        diwne.diwne2workArea(nodeRectDiwne.Min).x, diwne.diwne2workArea(nodeRectDiwne.Min).y,
		                        diwne.diwne2workArea(nodeRectDiwne.Max).x, diwne.diwne2workArea(nodeRectDiwne.Max).y,
		                        diwne.diwne2screen(nodeRectDiwne.Min).x, diwne.diwne2screen(nodeRectDiwne.Min).y,
		                        diwne.diwne2screen(nodeRectDiwne.Max).x, diwne.diwne2screen(nodeRectDiwne.Max).y)
		                .c_str());
	});                /* close of macro */
	ImGui::EndGroup(); /* End of node */
	ImGui::PopID();
}

// bool Node::afterEndDiwne()
//{
//	if (m_selectable)
//	{
//		bool prev_selected = m_selected;
//		if (diwne.getDiwneActionPreviousFrame() == DIWNE::DiwneAction::SelectionRectFull ||
//		    diwne.getDiwneAction() == DIWNE::DiwneAction::SelectionRectFull)
//		{
//			setSelected(diwne.getSelectionRectangleDiwne().Contains(getRectDiwne()) ? true
//			            : diwne.m_allowUnselectingNodes                                 ? false
//			                                                                            : m_selected);
//		}
//		else if (diwne.getDiwneActionPreviousFrame() == DIWNE::DiwneAction::SelectionRectTouch ||
//		         diwne.getDiwneAction() == DIWNE::DiwneAction::SelectionRectTouch)
//		{
//			setSelected(diwne.getSelectionRectangleDiwne().Overlaps(getRectDiwne()) ? true
//			            : diwne.m_allowUnselectingNodes                                 ? false
//			                                                                            : m_selected);
//		}
//		if (m_selected)
//		{
//			diwne.m_renderer->AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max,
// diwne.mp_settingsDiwne->itemSelectedBorderColor, diwne.mp_settingsDiwne->selectionRounding,
// ImDrawFlags_RoundCornersAll, diwne.mp_settingsDiwne->itemSelectedBorderThicknessDiwne);
//		}
//	}
//
//	/* always block interactions with other nodes */
//	ImGui::SetCursorScreenPos(diwne.diwne2screen(getNodePositionDiwne()));
//	ImGui::InvisibleButton("IBBlockingOtherImGuiInteractions", getRectDiwne().GetSize() * diwne.getWorkAreaZoom());
//
//	return DiwneObject::afterEndDiwne();
// }

bool Node::setSelected(const bool selected)
{
	bool prevSelected = m_selected;
	DiwneObject::setSelected(selected);
	if (prevSelected != m_selected)
	{
		// TODO: Uncomment
		//		if (m_selected)
		//			processSelect();
		//		else
		//			processUnselect();
		diwne.m_takeSnap = true;
	}
	return m_selected;
}
void Node::afterDrawDiwne(DrawInfo& context)
{
	// Adding an invisible ImGui blocking button to represent the logically opaque background of the node.
	// This needs to be done BEFORE processing interactions as the default implementation relies on ImGui::IsItemHovered
	ImGui::SetCursorScreenPos(diwne.diwne2screen(getNodePositionDiwne())); // TODO: Use m_rect instead
	ImGui::InvisibleButton("DiwneNodeBlockingButton", getRectDiwne().GetSize() * diwne.getWorkAreaZoom());
	m_internalHover = ImGui::IsItemHovered();
	DiwneObject::afterDrawDiwne(context);
}

// TODO: (DR) process/processUnselect seem to always return true, what is the purpose of the return value?
// bool Node::processSelect()
//{
//	diwne.setNodesSelectionChanged(true);
//	return true;
//}
//
// bool Node::processUnselect()
//{
//	diwne.setNodesSelectionChanged(true);
//	return true;
//}
void Node::onDrag(DrawInfo& context, bool dragStart, bool dragEnd)
{
	DiwneObject::onDrag(context, dragStart, dragEnd);
	move(diwne.bypassGetMouseDelta() / diwne.getWorkAreaZoom());
}

} /* namespace DIWNE */
