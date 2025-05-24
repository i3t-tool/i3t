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
