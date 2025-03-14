#pragma once

#include "DIWNE/Basic/BasicNode.h"
#include "DIWNE/Core/Elements/Containers/NodeContainer.h"

namespace DIWNE
{
class NodeEditor;
/**
 * Dummy container node used for testing. Doesn't actually display any child nodes.
 */
class DummyContainerNode : public BasicNode, public NodeContainer
{
public:
	DummyContainerNode(NodeEditor& editor, std::string label) : BasicNode(editor, label), NodeContainer(this) {}

	void onDestroy(bool logEvent) override
	{
		for (auto& node : m_nodes)
			node->destroy(logEvent);
		Node::onDestroy(logEvent);
	}
};
} // namespace DIWNE
