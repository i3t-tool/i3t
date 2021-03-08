#include "WorkspaceMatrixTranslation.h"

WorkspaceMatrixTranslation::WorkspaceMatrixTranslation(ImTextureID headerBackground, std::string headerLabel)
    : WorkspaceMatrix4x4(headerBackground, headerLabel, Builder::createTransform<Core::Translation>() )
{
}

void WorkspaceMatrixTranslation::drawData(util::NodeBuilder& builder)
{
    drawDataFull(builder); /* \todo JH here will be switch between different scale of view */
}

