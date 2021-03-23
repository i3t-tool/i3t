#include "WorkspaceMatrixFree.h"

WorkspaceMatrixFree::WorkspaceMatrixFree(ImTextureID headerBackground, WorkspaceMatrixFreeArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.viewScale=args.viewScale, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceMatrixFree::WorkspaceMatrixFree(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Builder::createNode<ENodeType::Matrix>(), headerLabel, nodeLabel){
}

void WorkspaceMatrixFree::drawData(util::NodeBuilder& builder)
{
	drawDataFull(builder); /* \todo JH here will be switch between different scale of view */
}

