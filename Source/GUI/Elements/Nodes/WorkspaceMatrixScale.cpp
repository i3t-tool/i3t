#include "WorkspaceMatrixScale.h"

WorkspaceMatrixScale::WorkspaceMatrixScale(ImTextureID headerBackground, WorkspaceMatrixScaleArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.viewScale=args.viewScale, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{
}

WorkspaceMatrixScale::WorkspaceMatrixScale(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, nodebase, headerLabel, nodeLabel){
}

void WorkspaceMatrixScale::drawData(util::NodeBuilder& builder)
{
    drawDataFull(builder); /* \todo JH here will be switch between different scale of view */
}


