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
#include "GUI/Workspace/WorkspaceModule.h"
#include "State/NodeDeserializer.h"
#include "State/SerializationVisitor.h"
#include "Viewport/entity/nodes/SceneModel.h"
#include "WorkspaceDiwne.h"

namespace Workspace
{
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
	for (auto& node : nodes)
	{
		node->translate(ImVec2(offset, offset));
	}
	SerializationVisitor serializer(*memento);
	serializer.dump(nodes);
	for (auto& node : nodes)
	{
		node->translate(ImVec2(-offset, -offset));
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
	WorkspaceModule::g_editor->shiftNodesToEnd(newNodes);
}

static auto findNode(Core::ID id) -> Result<Ptr<Workspace::CoreNode>, Error>
{
	auto& nodeEditor = I3T::getWorkspace().getNodeEditor();
	return nodeEditor.getNode<GuiNode>(id);
};

bool Tools::plug(Core::ID from, int fromIdx, Core::ID to, int toIdx)
{
	auto fromNode = findNode(from);
	auto toNode = findNode(to);

	if (!fromNode || !toNode)
	{
		return false;
	}

	return WorkspaceModule::connectNodesNoSave(fromNode.value(), toNode.value(), fromIdx, toIdx);
}
} // namespace Workspace
