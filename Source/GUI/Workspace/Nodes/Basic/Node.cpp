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

#include "GUI/I3TGui.h"

using namespace Workspace;

std::map<LevelOfDetail, std::string> Workspace::LevelOfDetailName = {{LevelOfDetail::Full, "Full"},
                                                                     {LevelOfDetail::SetValues, "Set values"},
                                                                     {LevelOfDetail::Label, "Label"},
                                                                     {LevelOfDetail::LightCycle, "Light cycle"}};

Node::Node(DIWNE::NodeEditor& diwne, std::string label) : DIWNE::BasicNode(diwne, label) {}
Node::Node(DIWNE::NodeEditor& diwne, std::string label) : DIWNE::BasicNode(diwne, label)
{
	LOG_EVENT_NODE_ADDED(m_labelDiwne, diwne.m_labelDiwne);
}

bool Node::allowDragStart() const
{
	// TODO: I feel like in general we should allow nodes to be dragged anywhere, and restrict specific node areas where
	//  it is not so appropriate. Eg. block dragging in matrix data content, but drag a model node anywhere.
	// Restrict node dragging to only the top header (meaning it can only be moved by the header)
	// && m_top.getRect().Contains(diwne.canvas().screen2diwne(diwne.input().bypassGetMousePos()));
	return DiwneObject::allowDragStart();
}

void Node::drawMenuDelete()
{
	if (I3TGui::MenuItemWithLog(_t("Delete"), "Delete"))
	{
		destroy();
	}
}

void Node::popupContent(DIWNE::DrawInfo& context)
{
	drawMenuDelete();
}
