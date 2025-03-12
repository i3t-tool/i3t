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

	/// Returns a iterable range to the nodes of this container.
	NodeRange<> getNodes() const override;
	/// Returns the underlying std::vector node list of the container.
	NodeList& getNodeList() override;

	/**
	 * Adds a node to the end of the list (O(1))
	 * The new node has this container set as its parent.
	 * Triggers onNodeAdd() with the new node.
	 */
	void addNode(const std::shared_ptr<Node>& node);

	/**
	 * Replaces an existing node with a new one. Must first find the old node (O(n)).
	 * The new node has this container set as its parent. The old node is destroyed.
	 * Triggers onNodeRemove() callback with a pointer to the old node
	 * and then onNodeAdd() with the new node.
	 * @return false if the node isn't found
	 */
	bool replaceNode(const std::shared_ptr<Node>& oldNode, const std::shared_ptr<Node>& newNode);

	/**
	 * Removes a node from the container. Must first find it (O(n)).
	 * This operation triggers shifting of all subsequent nodes by one position (O(n) worst case).
	 * Triggers onNodeRemove() callback with a pointer to the removed node.
	 * @return false if the node isn't found
	 */
	bool removeNode(const std::shared_ptr<Node>& node);

	/**
	 * Adds a node at a specified index.
	 * The new node has this container set as its parent.
	 * This operation triggers shifting of all subsequent nodes by one position (O(n) worst case) and reallocation.
	 * Triggers onNodeAdd() with the new node.
	 */
	virtual void addNodeAt(const std::shared_ptr<Node>& node, int index);
	/**
	 * Replaces node at a specified index with a different one. An O(1) operation.
	 * The new node has this container set as its parent.
	 * The old node is destroyed.
	 * Triggers onNodeRemove() callback with a pointer to the old node
	 * and then onNodeAdd() with the new node.
	 */
	virtual void replaceNodeAt(const std::shared_ptr<Node>& node, int index);
	/**
	 * Removes a node at a specified index from the container.
	 * This operation triggers shifting of all subsequent nodes by one position (O(n) worst case).
	 * Triggers onNodeRemove() callback with a pointer to the removed node.
	 */
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
