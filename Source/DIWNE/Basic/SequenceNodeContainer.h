#pragma once

#include "BasicNode.h"
#include "DIWNE/Core/Elements/Containers/NodeDropZone.h"

namespace DIWNE
{
class SequenceNodeContainer : public BasicNode, public INodeContainer
{
protected:
	std::shared_ptr<NodeDropZone> m_dropZone = std::make_shared<NodeDropZone>(diwne, this);

public:
	SequenceNodeContainer(NodeEditor& editor, std::string label = "SequenceNodeContainer");

	void centerContent(DrawInfo& context) override;

	NodeRange<> getNodes() const override;
	NodeList& getNodeList() override;

	NodeDropZone& getDropZone();

	void onDestroy(bool logEvent) override;
};
} // namespace DIWNE
