#pragma once
#include "WorkspaceMatrix4x4.h"

class WorkspaceMakeEulerY : public WorkspaceMatrix4x4
{
public:
    WorkspaceMakeEulerY();

    bool drawDataSetValues(DIWNE::Diwne &diwne);

    float get00();
    float get02();
    float get20();
    float get22();
};
