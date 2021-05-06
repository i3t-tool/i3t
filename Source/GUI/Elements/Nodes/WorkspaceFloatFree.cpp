#include "WorkspaceFloatFree.h"

WorkspaceFloatFree::WorkspaceFloatFree(ImTextureID headerBackground, WorkspaceFloatFreeArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceFloatFree::WorkspaceFloatFree(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::Float>(), headerLabel, nodeLabel)
{}

void WorkspaceFloatFree::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder,0);
}
