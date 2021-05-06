#include "WorkspaceVector3CrossVector3.h"

WorkspaceVector3CrossVector3::WorkspaceVector3CrossVector3(ImTextureID headerBackground, WorkspaceVector3CrossVector3Args const& args)
    : WorkspaceVector3(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVector3CrossVector3::WorkspaceVector3CrossVector3(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector3(headerBackground, Core::Builder::createNode<ENodeType::Vector3CrossVector3>(), headerLabel, nodeLabel)
{}

void WorkspaceVector3CrossVector3::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder,0);
}

