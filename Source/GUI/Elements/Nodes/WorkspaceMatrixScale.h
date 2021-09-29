#pragma once
#include "WorkspaceMatrix4x4.h"

class WorkspaceMatrixScale : public WorkspaceMatrix4x4
{
public:
    WorkspaceMatrixScale();

    bool drawDataSetValues(DIWNE::Diwne &diwne);
};
