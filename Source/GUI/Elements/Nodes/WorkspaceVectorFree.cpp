#include "WorkspaceVectorFree.h"

WorkspaceVectorFree::WorkspaceVectorFree(ImTextureID headerBackground, WorkspaceVectorFreeArgs const& args)
    : WorkspaceVector4(headerBackground, {.viewScale=args.viewScale, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceVectorFree::WorkspaceVectorFree(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector4(headerBackground, Builder::createNode<ENodeType::Vector4>(), headerLabel, nodeLabel)
{}

void WorkspaceVectorFree::drawData(util::NodeBuilder& builder)
{
	drawDataFull(builder); /* \todo JH here will be switch between different scale of view */
}

