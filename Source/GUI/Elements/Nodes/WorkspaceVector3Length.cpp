#include "WorkspaceVector3Length.h"

WorkspaceVector3Length::WorkspaceVector3Length(ImTextureID headerBackground, WorkspaceVector3LengthArgs const& args)
    : WorkspaceFloat(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVector3Length::WorkspaceVector3Length(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::Vector3Length>(), headerLabel, nodeLabel)
{}

void WorkspaceVector3Length::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}

