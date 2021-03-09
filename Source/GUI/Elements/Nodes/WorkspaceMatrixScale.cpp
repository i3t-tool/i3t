#include "WorkspaceMatrixScale.h"

WorkspaceMatrixScale::WorkspaceMatrixScale(ImTextureID headerBackground, std::string headerLabel, WorkspaceViewScale viewScale)
    : WorkspaceMatrix4x4(headerBackground, headerLabel, Builder::createTransform<Core::Scale>(), viewScale )
{
}

void WorkspaceMatrixScale::drawData(util::NodeBuilder& builder)
{
    drawDataFull(builder); /* \todo JH here will be switch between different scale of view */
}


