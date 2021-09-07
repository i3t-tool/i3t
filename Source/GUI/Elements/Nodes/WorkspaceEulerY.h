//
// Created by Sofie on 29.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

class WorkspaceEulerY : public WorkspaceMatrix4x4
{
public:
WorkspaceEulerY();

    bool drawDataSetValues(DIWNE::Diwne &diwne);
};
