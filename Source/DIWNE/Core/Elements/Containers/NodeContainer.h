#pragma once

#include <memory>
#include <vector>

#include "INodeContainer.h"

namespace DIWNE
{
class Node;

/**
 * Basic implementation of the INodeContainer interface.
 */
class NodeContainer : public INodeContainer
{
protected:
	std::vector<std::shared_ptr<Node>> m_nodes;

public:
	NodeRange<> getNodes() const override;
	std::vector<std::shared_ptr<Node>>& getNodeList() override;

	void addNode(const std::shared_ptr<Node>& node);
	void removeNode(const std::shared_ptr<Node>& node);
};

} // namespace DIWNE
