/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Tools.h"

#include <queue>

#include "Core/Nodes/Id.h"

#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"
#include "GUI/Workspace/Nodes/Model.h"
#include "State/NodeDeserializer.h"
#include "State/SerializationVisitor.h"
#include "Viewport/entity/nodes/SceneModel.h"
#include "WorkspaceDiwne.h"

using namespace Workspace;

std::optional<Ptr<GuiNode>> Tools::findNodeById(const std::vector<Ptr<GuiNode>>& nodes, Core::ID id)
{
	for (const auto& node : nodes)
		if (node->getNodebase()->getId() == id)
			return node;

	return std::nullopt;
}

Memento* Tools::copyNodes(const std::vector<Ptr<GuiNode>>& nodes, float offset)
{
	Memento* memento = new Memento();

	SerializationVisitor serializer(*memento);
	for (auto node : nodes)
	{
		node->move(ImVec2(-offset, -offset));
	}
	serializer.dump(nodes);
	for (auto node : nodes)
	{
		node->move(ImVec2(offset, offset));
	}
	return memento;
}

void Tools::pasteNodes(const Memento& memento)
{
	auto newNodes = NodeDeserializer::createFrom(memento);
	for (const auto& node : newNodes)
	{
		node->setSelected(true);
	}
}

void Tools::duplicateNode(const Ptr<GuiNode>& node, float offset)
{
	// TODO - DUPLICATES BEHIND NODE INSTEAD OF INFRONT
	pasteNodes(*copyNodes({node}, offset));
}

int Tools::numberOfCharWithDecimalPoint(float value, int numberOfVisibleDecimal)
{
	int border = 10, result = 1, int_value;

	if (value < 0)
	{
		value = -value;
	}
	result++; /* always space for sign to avoid changing size of / alternatively
	             move it inside if above */

	int_value = (int) value;
	while (int_value >= border)
	{
		result++;
		border *= 10;
	}

	return result + (numberOfVisibleDecimal > 0 ? numberOfVisibleDecimal + 1 : 0); /* +1 for decimal point */
}