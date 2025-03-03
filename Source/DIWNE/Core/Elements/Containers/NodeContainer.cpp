#include "NodeContainer.h"

#include "DIWNE/Core/diwne_iterators.h"

namespace DIWNE
{
NodeRange<> NodeContainer::getNodes() const
{
	return NodeRange<>(&m_nodes);
}
std::vector<std::shared_ptr<Node>>& NodeContainer::getNodeList()
{
	return m_nodes;
}
void NodeContainer::addNode(const std::shared_ptr<Node>& node)
{
	addNodeAt(node, m_nodes.size());
}
bool NodeContainer::removeNode(const std::shared_ptr<Node>& node)
{
	auto it = std::find(m_nodes.begin(), m_nodes.end(), node);
	if (it == m_nodes.end())
		return false;
	int index = std::distance(m_nodes.begin(), it);
	return removeNodeAt(index);
}
void NodeContainer::addNodeAt(const std::shared_ptr<Node>& node, int index)
{
	assert(node != nullptr);
	assert(index >= 0 && index <= m_nodes.size());
	m_nodes.insert(m_nodes.begin() + index, node);
	onNodeAdd(node.get(), index);
}
bool NodeContainer::removeNodeAt(int index)
{
	assert(index >= 0 && index < m_nodes.size());
	onNodeRemove(m_nodes.at(index).get(), index);
	m_nodes.erase(m_nodes.begin() + index);
	return true;
}

} // namespace DIWNE