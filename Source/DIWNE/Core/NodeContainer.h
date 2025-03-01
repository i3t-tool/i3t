#pragma once

#include <vector>

namespace DIWNE
{
class Node;
template <typename NodeType = Node>
class NodeRange;

class NodeContainer
{
public:
	virtual ~NodeContainer() = default;

	virtual NodeRange<> getNodes() const = 0;
};

} // namespace DIWNE
