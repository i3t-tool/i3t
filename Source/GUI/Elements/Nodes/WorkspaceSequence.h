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
#include "WorkspaceElementsWithCoreData.h"
#include "WorkspaceTransformation.h"

class WorkspaceSequence : public WorkspaceNodeWithCoreDataWithPins
{
private:
	bool m_isCameraSequence;
	ImVec4 m_tint = ImVec4(1, 1, 1, 1);

protected:
	int m_position_of_dummy_data = -1;
	ImVec2 m_sizeOfDummy = ImVec2(100, 1); /* \todo width from some setting */

	bool m_drawPins;
	std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceInnerTransformations;

public:
	WorkspaceSequence(DIWNE::Diwne& diwne, Ptr<Core::Node> nodebase = Core::GraphManager::createSequence(),
	                  bool drawPins = true, bool isCameraSequence = false);

	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<WorkspaceSequence>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	void setPostionOfDummyData(int positionOfDummyData = -1);

	void popupContentTracking();
	void popupContent();

	bool isSequence();
	int getInnerPosition(ImVec2 point);
	int getInnerPosition(std::vector<ImVec2> points);

	ImVec4 getTint()
	{
		return m_tint;
	}
	void setTint(ImVec4 tint)
	{
		m_tint = tint;
	}

	virtual bool allowDrawing();

	void popNode(Ptr<WorkspaceNodeWithCoreData> node);
	void pushNode(Ptr<WorkspaceNodeWithCoreData> node, int index = 0);

	void moveNodeToSequence(Ptr<WorkspaceNodeWithCoreData> dragedNode, int index = 0);
	void moveNodeToWorkspace(Ptr<WorkspaceNodeWithCoreData> dragedNode);

	void setNumberOfVisibleDecimal(int value);

	const std::vector<Ptr<WorkspaceNodeWithCoreData>>& getInnerWorkspaceNodes() const;
	std::optional<Ptr<WorkspaceNodeWithCoreData>> getTransform(int index) const;

	bool beforeContent();
	bool afterContent();
	bool topContent();
	bool middleContent();

	void drawMenuLevelOfDetail();

	int maxLengthOfData();
};
