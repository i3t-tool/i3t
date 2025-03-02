#pragma once

#include <memory>
#include <vector>

namespace DIWNE
{
class Node;
template <typename NodeType = Node>
class NodeRange;

class INodeContainer
{
public:
	virtual ~INodeContainer() = default;

	virtual NodeRange<> getNodes() const = 0;
	virtual std::vector<std::shared_ptr<Node>>& getNodeList() = 0;
};

} // namespace DIWNE
