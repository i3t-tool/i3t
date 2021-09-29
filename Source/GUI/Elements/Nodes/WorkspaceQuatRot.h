//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"
class WorkspaceQuatRot : public WorkspaceQuat
{
public:
    WorkspaceQuatRot();


    bool drawDataFull(DIWNE::Diwne &diwne, int index);

};

