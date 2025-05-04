/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "DIWNE/Core/Elements/Containers/INodeContainer.h"
#include "DIWNE/Core/Elements/Containers/NodeDropZone.h"

#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"

namespace Workspace
{
class Sequence : public CoreNodeWithPins, public DIWNE::INodeContainer
{
	using Super = CoreNodeWithPins;

protected:
	class SequenceDropZone;

	bool m_isCameraSequence;

public:
	/// Custom DIWNE::NodeDropZone that only accepts TransformationBase nodes.
	std::shared_ptr<SequenceDropZone> m_dropZone = std::make_shared<SequenceDropZone>(diwne, this);

public:
	Sequence(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase = Core::GraphManager::createSequence(),
	         bool isCameraSequence = false);

	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<Sequence>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	DIWNE::NodeRange<> getNodes() const override;
	DIWNE::NodeList& getNodeList() override;

	DIWNE::NodeRange<CoreNode> getInnerWorkspaceNodes();
	std::optional<Ptr<CoreNode>> getTransform(int index) const;

	void moveNodeToSequence(Ptr<CoreNode> dragedNode, int index = 0);
	void moveNodeToWorkspace(Ptr<CoreNode> dragedNode);

	void setNumberOfVisibleDecimal(int value) override;
	int maxLengthOfData() override;

	virtual bool allowDrawing() override;

	void begin(DIWNE::DrawInfo& context) override;
	void centerContent(DIWNE::DrawInfo& context) override;
	void afterDraw(DIWNE::DrawInfo& context) override;

	void drawInputPins(DIWNE::DrawInfo& context) override;
	void drawOutputPins(DIWNE::DrawInfo& context) override;

	void popupContent(DIWNE::DrawInfo& context) override;
	void popupContentTracking();
	void popupContentReferenceSpace();

	void drawMenuLevelOfDetail() override;

	void onDestroy(bool logEvent) override;

	bool isCameraSequence() const;

protected:
	class SequenceDropZone : public DIWNE::NodeDropZone
	{
	public:
		Sequence* m_sequence;

		SequenceDropZone(DIWNE::NodeEditor& diwne, Sequence* sequence);

	protected:
		void drawEmptyContent(DIWNE::DrawInfo& context) override;

	public:
		void onNodeAdd(DIWNE::Node* node, int index) override;
		void onNodeRemove(std::shared_ptr<DIWNE::Node> node, int index) override;
		bool acceptNode(DIWNE::Node* node) override;
	};
};
} // namespace Workspace