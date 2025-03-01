#include "BasicNodeContainer.h"

#include "DIWNE/Core/diwne_iterators.h"

namespace DIWNE
{
DIWNE::NodeRange<> BasicNodeContainer::getNodes() const
{
	return NodeRange<>(&m_childNodes);
}
void BasicNodeContainer::addNode(const std::shared_ptr<Node>& node)
{
	m_childNodes.push_back(node);
}
void BasicNodeContainer::removeNode(const std::shared_ptr<Node>& node)
{
	m_childNodes.erase(std::remove(m_childNodes.begin(), m_childNodes.end(), node), m_childNodes.end());
}
} // namespace DIWNE
