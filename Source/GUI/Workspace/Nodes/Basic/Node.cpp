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

Node::Node(DIWNE::NodeEditor& diwne, DIWNE::ID id, std::string const topLabel, std::string const middleLabel)
    : DIWNE::Node(diwne, id), m_topLabel(topLabel), m_middleLabel(middleLabel), m_removeFromWorkspaceWindow(false)
{}

Node::~Node()
{
	diwne.m_takeSnap = true;
}

bool Node::beforeContent()
{
	/* whole node background */
	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_bottomRectDiwne.Max, I3T::getTheme().get(EColor::NodeBg),
	                         I3T::getTheme().get(ESize::Nodes_Rounding), ImDrawFlags_RoundCornersAll);
	return false;
}

bool Node::topContent()
{
	bool interaction_happen = false;

	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max, I3T::getTheme().get(EColor::NodeHeader),
	                         I3T::getTheme().get(ESize::Nodes_Rounding), ImDrawFlags_RoundCornersTop);
	ImGui::Dummy(ImVec2(ImGui::GetStyle().ItemSpacing.x, 1));
	ImGui::SameLine();
	ImGui::TextUnformatted(m_topLabel.c_str());

	return interaction_happen;
}

bool Node::middleContent()
{
	bool interaction_happen = false;

	ImGui::TextUnformatted(m_middleLabel.c_str());

	return interaction_happen;
}

bool Node::leftContent()
{
	return false;
}
bool Node::rightContent()
{
	return false;
}
bool Node::bottomContent()
{
	return false;
}

bool Node::bypassFocusForInteractionAction()
{
	return (m_isHeld || m_topRectDiwne.Contains(diwne.screen2diwne(diwne.bypassGetMousePos())));
}

void Node::deleteAction()
{
	// TODO: (DR) A little confused why we don't use the superclass m_toDelete boolean flag for this but whatever
	m_removeFromWorkspaceWindow = true;
}

void Node::drawMenuDelete()
{
	if (ImGui::MenuItem("Delete", "Delete"))
	{
		deleteActionDiwne();
	}
}

void Node::popupContent()
{
	drawMenuDelete();
}
