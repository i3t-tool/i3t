#include "WorkspaceFloatFree.h"
/*
WorkspaceFloatFree::WorkspaceFloatFree(ImTextureID headerBackground, WorkspaceVectorFreeArgs const& args)
    : WorkspaceFloat(headerBackground, { .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}*/

WorkspaceFloatFree::WorkspaceFloatFree(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Builder::createNode<ENodeType::Float>(), headerLabel, nodeLabel)
{}

void WorkspaceFloatFree::drawData(util::NodeBuilder& builder)
{
    drawDataFull(builder); 
}

