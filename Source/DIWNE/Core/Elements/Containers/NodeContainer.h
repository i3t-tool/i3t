#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "INodeContainer.h"

namespace DIWNE
{
class Node;
class DiwneObject;

/**
 * Basic implementation of a node container, eg. an object owning other nodes.
 * It is used to expand functionality of an existing DiwneObject, which is meant to inherit this class
 * using multiple inheritance to add container functionality.
 *
 * \warning NodeContainer requires its DiwneObject owner to call purgeNodes() every frame if automatic removal of nodes
 * is needed. Otherwise when a node is added it must be manually removed from its previous container.
 */
class NodeContainer : public INodeContainer
{
protected:
	NodeList m_nodes;
	DiwneObject* m_owner; ///< Diwne object associated with this node container.

public:
	/// Constructs the node container for a specific DiwneObject that it represents.
	NodeContainer(DiwneObject* owner);

	NodeRange<> getNodes() const override;
	NodeList& getNodeList() override;

	/// Adds a node to the end of the list
	void addNode(const std::shared_ptr<Node>& node);
	void replaceNode(const std::shared_ptr<Node>& oldNode, const std::shared_ptr<Node>& newNode);
	bool removeNode(const std::shared_ptr<Node>& node);

	/// Adds a node at an index
	virtual void addNodeAt(const std::shared_ptr<Node>& node, int index);
	virtual void replaceNodeAt(const std::shared_ptr<Node>& node, int index);
	virtual void removeNodeAt(int index);

	/// Erases objects marked for deletion or removal from the container.
	/// This method needs to be called every frame by the container owner.
	void purgeNodes();
protected:
	/**
	 * Called after a node is inserted into the container.
	 * @param node Raw pointer to the just inserted node.
	 * @param index Index the node was inserted at.
	 */
	virtual void onNodeAdd(Node* node, int index){};

	/**
	 * Called after a node is removed from the container.
	 * An owning pointer is passed as its ref count could be one.
	 * @param node Owning shared pointer to the removed node.
	 * @param index Index the node was removed from (potentially invalid).
	 */
	virtual void onNodeRemove(std::shared_ptr<Node> node, int index){};
};

} // namespace DIWNE
