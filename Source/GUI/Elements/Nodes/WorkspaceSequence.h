#pragma once
#include "WorkspaceElementsWithCoreData.h"
#include "WorkspaceTransformation.h"

class WorkspaceSequence : public WorkspaceNodeWithCoreDataWithPins
{
private:
	bool m_isCameraSequence;

protected:
	int m_position_of_dummy_data = -1;
	ImVec2 m_sizeOfDummy = ImVec2(100, 1); /* \todo width from some setting */

	bool m_drawPins;
	std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceInnerTransformations;

public:
	WorkspaceSequence(DIWNE::Diwne& diwne, Ptr<Core::NodeBase> nodebase = Core::Builder::createSequence(),
	                  bool drawPins = true, bool isCameraSequence = false);

	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<WorkspaceSequence>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	void setPostionOfDummyData(int positionOfDummyData = -1);

	bool isSequence();
	int getInnerPosition(ImVec2 point);
	int getInnerPosition(std::vector<ImVec2> points);

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

	int maxLenghtOfData();
};
