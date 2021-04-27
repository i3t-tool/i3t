#include "WorkspaceVector3SubVector3.h"

WorkspaceVector3SubVector3::WorkspaceVector3SubVector3(ImTextureID headerBackground, WorkspaceVector3SubVector3Args const& args)
    : WorkspaceVector3(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVector3SubVector3::WorkspaceVector3SubVector3(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector3(headerBackground, Core::Builder::createNode<ENodeType::Vector3SubVector3>(), headerLabel, nodeLabel)
{}

void WorkspaceVector3SubVector3::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}

