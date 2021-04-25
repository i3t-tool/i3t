#include "WorkspaceMixVector3.h"

WorkspaceMixVector3::WorkspaceMixVector3(ImTextureID headerBackground, WorkspaceMixVector3Args const& args)
    : WorkspaceVector3(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMixVector3::WorkspaceMixVector3(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector3(headerBackground, Core::Builder::createNode<ENodeType::MixVector3>(), headerLabel, nodeLabel)
{}

void WorkspaceMixVector3::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}

