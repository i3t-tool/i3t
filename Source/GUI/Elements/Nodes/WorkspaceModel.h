#pragma once

#include "WorkspaceElementsWithCoreData.h"

class WorkspaceModel : public WorkspaceNodeWithCoreDataWithPins
{
public:
  WorkspaceModel();

	//===-- Double dispatch ---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		// visitor.visit(std::static_pointer_cast<WorkspaceSequence>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

  bool drawDataFull(DIWNE::Diwne& diwne, int index);
	int maxLenghtOfData();
	bool middleContent(DIWNE::Diwne& diwne);
	void drawMenuLevelOfDetail();

private:
	int m_currentModelIdx = 0;

	void init();
};
