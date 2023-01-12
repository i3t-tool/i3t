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

	return nullptr;
}

Memento copyNodes(const std::vector<Ptr<GuiNode>>& nodes)
{
	Memento memento;

	SerializationVisitor serializer(memento, true);
	serializer.dump(nodes);

	return memento;
}

void pasteNodes(const Memento& memento) { NodeDeserializer::createFrom(memento); }

//----------------------------------------------------------------------------//

Tracking::Tracking(Ptr<WorkspaceSequence> beginSequence)
{
	std::vector<Ptr<Core::IModelProxy>> models;

	// Get model connected to the sequence.
	const auto& modelOutput = beginSequence->getOutputs()[0];

	// Ugly workaround since workspace output pin has no references to connected nodes.
	const auto& connectedModelsInputs = modelOutput->getCorePin().getOutComponents();

	for (const auto& connectedModelInput : connectedModelsInputs)
	{
		const auto coreModelNode = connectedModelInput->getOwner();

		auto& windowManager = App::getModule<UIModule>().getWindowManager();
		auto& nodeEditor = windowManager.getWindowPtr<WorkspaceWindow>()->getNodeEditor();
		auto maybeNode = findNodeById(nodeEditor.m_workspaceCoreNodes, coreModelNode->getId());
		if (auto node = maybeNode.value())
		{
			models.push_back(std::make_shared<TrackingModelProxy>((WorkspaceModel&)node));
		}
	}

	m_internal = Core::MatrixTracker(beginSequence->getNodebase()->as<Core::Sequence>(), models);
}

void Tracking::setParam(float value) { m_internal.setParam(value); }

float Tracking::getParam() const { return m_internal.getParam(); }
