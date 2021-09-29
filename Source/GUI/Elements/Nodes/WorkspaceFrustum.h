//
// Created by Sofie on 30.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

class WorkspaceFrustum : public WorkspaceMatrix4x4
{
public:
    WorkspaceFrustum();

    bool drawDataSetValues(DIWNE::Diwne &diwne);
};

