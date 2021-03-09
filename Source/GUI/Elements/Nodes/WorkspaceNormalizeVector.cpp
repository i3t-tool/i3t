#include "WorkspaceNormalizeVector.h"

WorkspaceNormalizeVector::WorkspaceNormalizeVector(ImTextureID headerBackground, std::string headerLabel, WorkspaceViewScale viewScale)
    : WorkspaceVector4(headerBackground, headerLabel, Builder::createNode<ENodeType::NormalizeVector>(), viewScale)
{
}

void WorkspaceNormalizeVector::drawData(util::NodeBuilder& builder)
{
    drawDataFull(builder); /* \todo JH here will be switch between different scale of view */
}



