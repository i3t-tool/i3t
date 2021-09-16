#include "WorkspaceVector3Free.h"

WorkspaceVector3Free::WorkspaceVector3Free(ImTextureID headerBackground, WorkspaceVector3FreeArgs const& args)
    : WorkspaceVector3(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVector3Free::WorkspaceVector3Free(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector3(headerBackground, Core::Builder::createNode<ENodeType::Vector3ToVector3>(), headerLabel, nodeLabel)
{}

void WorkspaceVector3Free::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder,0);
}

