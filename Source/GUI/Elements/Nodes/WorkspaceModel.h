#pragma once

#include "WorkspaceElementsWithCoreData.h"

class WorkspaceModel : public WorkspaceNodeWithCoreData
{
public:
  WorkspaceModel();

  bool drawDataFull(DIWNE::Diwne& diwne, int index);
	int maxLenghtOfData(int index);

private:
	int m_currentModelIdx = 0;

	void init();
};
