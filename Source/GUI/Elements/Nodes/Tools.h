#pragma once

#include "Core/Defs.h"
#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Id.h"
#include "Core/Nodes/Tracking.h"
#include "State/NodeVisitor.h"
#include "State/Stateful.h"

std::optional<Ptr<GuiNode>> findNodeById(const std::vector<Ptr<GuiNode>>& nodes, Core::ID id);

Memento copyNodes(const std::vector<Ptr<GuiNode>>& nodes);

void duplicateNode(const Ptr<GuiNode>& node);

void pasteNodes(const Memento& memento);

//------------------------------------------------------------------------------------------------//

class WorkspaceModelProxy : public Core::IModelProxy
{
public:
	WorkspaceModelProxy(Ptr<WorkspaceModel> model);
	~WorkspaceModelProxy() override;

	void update(const glm::mat4& transform) override;
	Ptr<Core::Model> getModel() override;

private:
	Ptr<WorkspaceModel> m_model;
};
