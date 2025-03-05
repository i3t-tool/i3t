#pragma once

#include "DIWNE/Basic/BasicNode.h"
#include "DIWNE/Core/Elements/Containers/NodeContainer.h"

namespace DIWNE
{
class NodeEditor;
/**
 * Dummy container node used for testing. Doesn't actually display any child nodes.
 */
class DummyContainerNode : public BasicNode, public NodeContainer
{
public:
	DummyContainerNode(NodeEditor& editor, std::string label) : BasicNode(editor, label), NodeContainer(this) {}
};
} // namespace DIWNE
