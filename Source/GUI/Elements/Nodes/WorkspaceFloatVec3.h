//
// Created by Sofie on 06.05.2021.
//

#pragma once
#include "WorkspaceElementsWithCoreData.h"

class WorkspaceFloatVec3 : public WorkspaceNodeWithCoreData
{
public:
  WorkspaceFloatVec3(Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3>());
    virtual bool drawDataFull(DIWNE::Diwne& diwne, int index);
  int maxLenghtOfData(int index=0);

};

