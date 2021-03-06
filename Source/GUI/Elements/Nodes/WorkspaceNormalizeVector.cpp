#include "WorkspaceNormalizeVector.h"

WorkspaceNormalizeVector::WorkspaceNormalizeVector(ImTextureID headerBackground, std::string headerLabel = "default NormalizeVector header")
    : WorkspaceVector4(headerBackground, headerLabel, Builder::createNode<ENodeType::NormalizeVector>())
{
}

void WorkspaceNormalizeVector::drawData(util::NodeBuilder& builder)
{
    drawDataFull(builder); /* \todo JH here will be switch between different scale of view */
}



