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
	m_dropZone->drawDiwne(context, m_drawMode);
	ImGui::Spacing();
}

DIWNE::NodeRange<> SequenceNodeContainer::getNodes() const
{
	return m_dropZone->getNodes();
}
NodeList& SequenceNodeContainer::getNodeList()
{
	return m_dropZone->getNodeList();
}
NodeDropZone& SequenceNodeContainer::getDropZone()
{
	return *m_dropZone;
}
void SequenceNodeContainer::onDestroy(bool logEvent)
{
	m_dropZone->destroy(logEvent);
	Node::onDestroy(logEvent);
}
} // namespace DIWNE
