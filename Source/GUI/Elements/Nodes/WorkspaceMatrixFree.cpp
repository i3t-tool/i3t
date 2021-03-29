#include "WorkspaceMatrixFree.h"

WorkspaceMatrixFree::WorkspaceMatrixFree(ImTextureID headerBackground, WorkspaceMatrixFreeArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceMatrixFree::WorkspaceMatrixFree(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Builder::createNode<ENodeType::Matrix>(), headerLabel, nodeLabel){
}

void WorkspaceMatrixFree::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}

