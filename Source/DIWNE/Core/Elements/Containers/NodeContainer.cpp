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
	m_nodes.push_back(node);
}
void NodeContainer::removeNode(const std::shared_ptr<Node>& node)
{
	m_nodes.erase(std::remove(m_nodes.begin(), m_nodes.end(), node), m_nodes.end());
}

} // namespace DIWNE