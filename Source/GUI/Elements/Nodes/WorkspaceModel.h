#pragma once

#include "WorkspaceElementsWithCoreData.h"

class WorkspaceModel : public WorkspaceNodeWithCoreDataWithPins
{
public:
  WorkspaceModel();

  bool drawDataFull(DIWNE::Diwne& diwne, int index);
	int maxLenghtOfData();
	bool middleContent(DIWNE::Diwne& diwne);
	void drawMenuLevelOfDetail();

private:
	int m_currentModelIdx = 0;

	void init();
};
