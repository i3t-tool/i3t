#include "Tools.h"

#include <queue>

#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Id.h"

#include "GUI/Elements/Nodes/WorkspaceElementsWithCoreData.h"
#include "GUI/Elements/Nodes/WorkspaceModel.h"
#include "GUI/Elements/Nodes/WorkspaceSequence.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "State/NodeDeserializer.h"
#include "State/SerializationVisitor.h"

std::optional<Ptr<GuiNode>> findNodeById(const std::vector<Ptr<GuiNode>>& nodes, Core::ID id)
{
	for (const auto& node : nodes)
		if (node->getNodebase()->getId() == id)
			return node;

	return std::nullopt;
}

Memento copyNodes(const std::vector<Ptr<GuiNode>>& nodes)
{
	Memento memento;

	SerializationVisitor serializer(memento, true);
	for (auto node : nodes){
		node->translateNodePositionDiwne(ImVec2(-10, -10));
	}
	serializer.dump(nodes);
	for (auto node : nodes){
		node->translateNodePositionDiwne(ImVec2(10, 10));
	}
	return memento;
}

void pasteNodes(const Memento& memento) { NodeDeserializer::createFrom(memento); }

void duplicateNode(const Ptr<GuiNode>& node) {
	std::vector<Ptr<GuiNode>> temp;
	temp.push_back(node);
	pasteNodes(copyNodes(temp));
}

//------------------------------------------------------------------------------------------------//

WorkspaceModelProxy::WorkspaceModelProxy(Ptr<WorkspaceModel> model)
{
	auto workspace = I3T::getWindowPtr<WorkspaceWindow>();
	m_model = std::make_shared<WorkspaceModel>(workspace->getNodeEditor());
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
