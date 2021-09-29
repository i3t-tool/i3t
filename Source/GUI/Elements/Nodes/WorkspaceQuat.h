//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceElementsWithCoreData.h"
class WorkspaceQuat : public WorkspaceNodeWithCoreData
{
public:
    WorkspaceQuat(Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatToQuat>());

  virtual bool drawDataFull(DIWNE::Diwne &diwne, int index);

  int maxLenghtOfData(int index=0);
};
