#include "WorkspaceShowVector3.h"

WorkspaceShowVector3::WorkspaceShowVector3(ImTextureID headerBackground, WorkspaceShowVector3Args const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceShowVector3::WorkspaceShowVector3(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::ShowVector3>(), headerLabel, nodeLabel)
{}

void WorkspaceShowVector3::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder,0);
}

