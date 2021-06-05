#include "WorkspaceVectorFree.h"

WorkspaceVectorFree::WorkspaceVectorFree(ImTextureID headerBackground, WorkspaceVectorFreeArgs const& args)
    : WorkspaceVector4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceVectorFree::WorkspaceVectorFree(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector4(headerBackground, Core::Builder::createNode<ENodeType::Vector4>(), headerLabel, nodeLabel)
{}

void WorkspaceVectorFree::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder,0);
}

