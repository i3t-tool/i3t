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

#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Id.h"

#include "GUI/Elements/Nodes/WorkspaceElementsWithCoreData.h"
#include "GUI/Elements/Nodes/WorkspaceModel.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "State/NodeDeserializer.h"
#include "State/SerializationVisitor.h"
#include "Viewport/entity/nodes/SceneModel.h"

std::optional<Ptr<GuiNode>> findNodeById(const std::vector<Ptr<GuiNode>>& nodes, Core::ID id)
{
	for (const auto& node : nodes)
		if (node->getNodebase()->getId() == id)
			return node;

	return std::nullopt;
}

Memento* copyNodes(const std::vector<Ptr<GuiNode>>& nodes, float offset)
{
	Memento* memento = new Memento();

	SerializationVisitor serializer(*memento);
	for (auto node : nodes)
	{
		node->translateNodePositionDiwne(ImVec2(-offset, -offset));
	}
	serializer.dump(nodes);
	for (auto node : nodes)
	{
		node->translateNodePositionDiwne(ImVec2(offset, offset));
	}
	return memento;
}

void pasteNodes(const Memento& memento)
{
	auto newNodes = NodeDeserializer::createFrom(memento);
	for (const auto& node : newNodes)
	{
		node->setSelected(true);
	}
}

void duplicateNode(const Ptr<GuiNode>& node, float offset)
{
	// TODO - DUPLICATES BEHIND NODE INSTEAD OF INFRONT
	pasteNodes(*copyNodes({node}, offset));
}

//------------------------------------------------------------------------------------------------//

WorkspaceModelProxy::WorkspaceModelProxy(Ptr<WorkspaceModel> model)
{
	auto workspace = I3T::getWindowPtr<WorkspaceWindow>();

	m_model = std::make_shared<WorkspaceModel>(workspace->getNodeEditor());
	const auto alias = model->viewportModel().lock()->getModel();
	m_model->viewportModel().lock()->setModel(alias);
}

WorkspaceModelProxy::~WorkspaceModelProxy()
{
	m_model = nullptr;
}

void WorkspaceModelProxy::update(const glm::mat4& transform)
{
	getModel()->m_modelMatrix = transform;
	getModel()->updateValues(0);
}

Ptr<Core::Model> WorkspaceModelProxy::getModel()
{
	return m_model->getNodebase()->as<Core::Model>();
}
