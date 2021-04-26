#include "WorkspaceNormalizeVector3.h"

WorkspaceNormalizeVector3::WorkspaceNormalizeVector3(ImTextureID headerBackground, WorkspaceNormalizeVector3Args const& args)
    : WorkspaceVector3(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceNormalizeVector3::WorkspaceNormalizeVector3(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector3(headerBackground, Core::Builder::createNode<ENodeType::NormalizeVector3>(), headerLabel, nodeLabel)
{}

void WorkspaceNormalizeVector3::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}

