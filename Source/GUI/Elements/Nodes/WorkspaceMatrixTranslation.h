#pragma once
#include "WorkspaceMatrix4x4.h"

class WorkspaceMatrixTranslation : public WorkspaceMatrix4x4
{
public:
   	WorkspaceMatrixTranslation();

    bool drawDataSetValues(DIWNE::Diwne &diwne);

};
