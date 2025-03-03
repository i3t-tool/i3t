#pragma once

#include <memory>
#include <vector>

#include "INodeContainer.h"

namespace DIWNE
{
class Node;

/**
 * Basic implementation of the INodeContainer interface.
 * Adds additional methods that can be useful.
 */
class NodeContainer : public INodeContainer
{
protected:
	std::vector<std::shared_ptr<Node>> m_nodes;

public:
	NodeRange<> getNodes() const override;
	std::vector<std::shared_ptr<Node>>& getNodeList() override;

	/// Adds a node to the end of the list
	void addNode(const std::shared_ptr<Node>& node);
	bool removeNode(const std::shared_ptr<Node>& node);


	/// Adds a node at an index
	virtual void addNodeAt(const std::shared_ptr<Node>& node, int index);
	virtual bool removeNodeAt(int index);

	virtual void onNodeAdd(Node* node, int index){};
	virtual void onNodeRemove(Node* node, int index){};
};

} // namespace DIWNE
