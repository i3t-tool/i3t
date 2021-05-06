#include "WorkspaceVector3AddVector3.h"

WorkspaceVector3AddVector3::WorkspaceVector3AddVector3(ImTextureID headerBackground, WorkspaceVector3AddVector3Args const& args)
    : WorkspaceVector3(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVector3AddVector3::WorkspaceVector3AddVector3(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector3(headerBackground, Core::Builder::createNode<ENodeType::Vector3AddVector3>(), headerLabel, nodeLabel)
{}

void WorkspaceVector3AddVector3::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder,0);
}

