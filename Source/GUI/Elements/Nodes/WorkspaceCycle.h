//
// Created by Sofie on 07.05.2021.
//

#pragma once
#include "WorkspaceElementsWithCoreData.h"
class WorkspaceCycle : public WorkspaceNodeWithCoreData
{
public:
  WorkspaceCycle();
	bool isCycle();

  bool drawDataFull(DIWNE::Diwne& diwne, int index);
  void drawMenuLevelOfDetail();

    int maxLenghtOfData(int index=0);
};

