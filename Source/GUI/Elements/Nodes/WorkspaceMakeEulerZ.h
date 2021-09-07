#pragma once
#include "WorkspaceMatrix4x4.h"
class WorkspaceMakeEulerZ : public WorkspaceMatrix4x4
{
public:
    WorkspaceMakeEulerZ();

    bool drawDataSetValues(DIWNE::Diwne &diwne);

    float get00();
    float get01();
    float get10();
    float get11();
};
