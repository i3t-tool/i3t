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

using namespace Workspace;

std::map<LevelOfDetail, std::string> Workspace::LevelOfDetailName = {{LevelOfDetail::Full, "Full"},
                                                                     {LevelOfDetail::SetValues, "Set values"},
                                                                     {LevelOfDetail::Label, "Label"},
                                                                     {LevelOfDetail::LightCycle, "Light cycle"}};

Node::Node(DIWNE::NodeEditor& diwne, std::string label) : DIWNE::BasicNode(diwne, label) {}

Node::~Node()
{
	diwne.m_takeSnap = true;
}

bool Node::bypassFocusForInteractionAction()
{
	return (m_isHeld || m_top.getRect().Contains(diwne.screen2diwne(diwne.m_input->bypassGetMousePos())));
}

void Node::drawMenuDelete()
{
	if (ImGui::MenuItem("Delete", "Delete"))
	{
		destroy();
	}
}

void Node::popupContent(DIWNE::DrawInfo& context)
{
	drawMenuDelete();
}

void Node::onDestroy(bool logEvent)
{
	m_removeFromWorkspaceWindow = true;
}
