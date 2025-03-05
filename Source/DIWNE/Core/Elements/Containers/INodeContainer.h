#pragma once

#include "DIWNE/Core/diwne_common.h"

namespace DIWNE
{
template <typename NodeType = Node>
class NodeRange;

/**
 * Elementary node container interface defining access to child nodes.
 * @see NodeContainer
 */
class INodeContainer
{
public:
	virtual ~INodeContainer() = default;

	virtual NodeRange<> getNodes() const = 0;
	virtual NodeList& getNodeList() = 0;
};

} // namespace DIWNE
