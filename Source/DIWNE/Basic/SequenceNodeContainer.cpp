/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
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
