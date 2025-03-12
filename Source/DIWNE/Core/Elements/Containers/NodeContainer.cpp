#include "NodeContainer.h"

#include "DIWNE/Core/diwne_iterators.h"

namespace DIWNE
{
NodeContainer::NodeContainer(DiwneObject* owner) : m_owner(owner) {}
NodeRange<> NodeContainer::getNodes() const
{
	return NodeRange<>(&m_nodes);
}
NodeList& NodeContainer::getNodeList()
{
	return m_nodes;
}
void NodeContainer::addNode(const std::shared_ptr<Node>& node)
{
	addNodeAt(node, m_nodes.size());
}
void NodeContainer::replaceNode(const std::shared_ptr<Node>& oldNode, const std::shared_ptr<Node>& newNode)
{
	auto it = std::find(m_nodes.begin(), m_nodes.end(), oldNode);
	if (it != m_nodes.end())
	{
		replaceNodeAt(newNode, std::distance(m_nodes.begin(), it));
	}
}
bool NodeContainer::removeNode(const std::shared_ptr<Node>& node)
{
	auto it = std::find(m_nodes.begin(), m_nodes.end(), node);
	if (it == m_nodes.end())
		return false;
	int index = std::distance(m_nodes.begin(), it);
	removeNodeAt(index);
	return true;
}
void NodeContainer::addNodeAt(const std::shared_ptr<Node>& node, int index)
{
	assert(node != nullptr);
	assert(index >= 0 && index <= m_nodes.size());

	// Change nodes parent to this container, its original container will remove it as it will no longer be its parent.
	node->setParentObject(m_owner);

	m_nodes.insert(m_nodes.begin() + index, node);

	onNodeAdd(node.get(), index);
}
void NodeContainer::replaceNodeAt(const std::shared_ptr<Node>& node, int index)
{
	assert(node != nullptr);
	assert(index >= 0 && index <= m_nodes.size());

	// Change nodes parent to this container, its original container will remove it as it will no longer be its parent.
	node->setParentObject(m_owner);

	auto it = m_nodes.begin() + index;
	std::shared_ptr<Node> nodeBckup = *it;
	if (!nodeBckup->isDestroyed())
		nodeBckup->destroy(false);
	*it = node;

	onNodeRemove(nodeBckup, index);
	onNodeAdd(node.get(), index);
}
void NodeContainer::removeNodeAt(int index)
{
	assert(index >= 0 && index < m_nodes.size());

	auto it = m_nodes.begin() + index;
	std::shared_ptr<Node> nodeBckup = *it;
	if (!nodeBckup->isDestroyed())
		nodeBckup->destroy(false);
	m_nodes.erase(it);

	onNodeRemove(nodeBckup, index);
}
void NodeContainer::purgeNodes()
{
	if (m_nodes.empty())
		return;

	// Helper vector used for temporary storage during purge
	static std::vector<std::pair<std::shared_ptr<Node>, int>> purgedNodes;
	static int index;
	index = 0;

	// We erase nodes that are destroyed or are no longer contained in this container (we're not their parent anymore)
	auto endIt = std::remove_if(m_nodes.begin(), m_nodes.end(), [this](const auto& node) -> bool {
		if (node->m_destroy || node->willBeRemovedFromContainer(this->m_owner))
		{
			purgedNodes.emplace_back(node, index);
			++index;
			return true;
		}
		++index;
		return false;
	});
	if (!purgedNodes.empty())
	{
		int purgedIndex = purgedNodes.size() - 1;
		int newEndIndex = std::distance(m_nodes.begin(), endIt);
		for (int i = m_nodes.size() - 1; i >= newEndIndex; --i)
		{
			m_nodes.pop_back();
			auto& purgedEntry = purgedNodes[purgedIndex--];
			onNodeRemove(purgedEntry.first, purgedEntry.second);
		}
	}
	purgedNodes.clear(); // Clear temporary vector and release pointers
}

} // namespace DIWNE