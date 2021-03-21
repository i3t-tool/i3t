#include "WorkspaceNormalizeVector.h"

WorkspaceNormalizeVector::WorkspaceNormalizeVector(ImTextureID headerBackground, WorkspaceNormalizeVectorArgs const& args)
    : WorkspaceVector4(headerBackground, {.viewScale=args.viewScale, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{
}

WorkspaceNormalizeVector::WorkspaceNormalizeVector(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase)
    : WorkspaceVector4(headerBackground, nodebase)
{
}

void WorkspaceNormalizeVector::drawData(util::NodeBuilder& builder)
{
    drawDataFull(builder); /* \todo JH here will be switch between different scale of view */
}



