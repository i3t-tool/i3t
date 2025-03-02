#include "SequenceNodeContainer.h"

#include "DIWNE/Core/diwne_iterators.h"

namespace DIWNE
{
SequenceNodeContainer::SequenceNodeContainer(NodeEditor& editor, std::string label) : BasicNode(editor, label)
{
	m_dropZone->setParentObject(this);
}

void SequenceNodeContainer::centerContent(DrawInfo& context)
{
	ImGui::Text("Content");
	m_dropZone->drawDiwne(context, m_drawMode2);
	ImGui::Spacing();
}

DIWNE::NodeRange<> SequenceNodeContainer::getNodes() const
{
	return m_dropZone->getNodes();
}
std::vector<std::shared_ptr<Node>>& SequenceNodeContainer::getNodeList()
{
	return m_dropZone->getNodeList();
}
} // namespace DIWNE
