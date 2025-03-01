#pragma once

#include "BasicNode.h"
#include "DIWNE/Core/NodeContainer.h"

namespace DIWNE
{
class BasicNodeContainer : public BasicNode, public NodeContainer
{
protected:
	std::vector<std::shared_ptr<Node>> m_childNodes;

public:
	NodeRange<> getNodes() const override;

	BasicNodeContainer(NodeEditor& editor, std::string label) : BasicNode(editor, label) {}

	void addNode(const std::shared_ptr<Node>& node);
	void removeNode(const std::shared_ptr<Node>& node);
};
} // namespace DIWNE
