//
// Created by Sofie on 15.05.2021.
//
#pragma once
#include "WorkspaceElementsWithCoreData.h"
class WorkspacePulse : public WorkspaceNodeWithCoreData
{
public:
  WorkspacePulse();

  bool drawDataFull(DIWNE::Diwne& diwne, int index);
  int maxLenghtOfData(int index=0);
};
