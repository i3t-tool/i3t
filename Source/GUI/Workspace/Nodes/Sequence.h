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

#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"

#include "TransformationBase.h"

namespace Workspace
{
class Sequence : public CoreNodeWithPins
{
private:
	bool m_isCameraSequence;
	ImVec4 m_tint = ImVec4(1, 1, 1, 1);

protected:
	int m_position_of_dummy_data = -1;
	ImVec2 m_sizeOfDummy = ImVec2(100, 1); /* \todo width from some setting */

	// bool m_drawPins;  // \todo (PF) was not used - remove
	std::vector<Ptr<CoreNode>> m_workspaceInnerTransformations;

public:
	Sequence(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase = Core::GraphManager::createSequence(),
	         /* bool drawPins = true, \todo (PF) was not used - remove */
	         bool isCameraSequence = false);

	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<Sequence>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	void setPostionOfDummyData(int positionOfDummyData = -1);

	void popupContentTracking();
	void popupContent(DIWNE::DrawInfo& context) override;

	bool isSequence();

	int getInnerPosition(std::vector<ImVec2> points);

	ImVec4 getTint()
	{
		return m_tint;
	}
	void setTint(ImVec4 tint)
	{
		m_tint = tint;
	}

	virtual bool allowDrawing() override;

	void popNode(Ptr<CoreNode> node);
	void pushNode(Ptr<CoreNode> node, int index = 0);

	void moveNodeToSequence(Ptr<CoreNode> dragedNode, int index = 0);
	void moveNodeToWorkspace(Ptr<CoreNode> dragedNode);

	void setNumberOfVisibleDecimal(int value) override;

	const std::vector<Ptr<CoreNode>>& getInnerWorkspaceNodes() const;
	std::optional<Ptr<CoreNode>> getTransform(int index) const;

	void centerContent(DIWNE::DrawInfo& context) override;

	void drawMenuLevelOfDetail() override;

	int maxLengthOfData() override;
};
} // namespace Workspace