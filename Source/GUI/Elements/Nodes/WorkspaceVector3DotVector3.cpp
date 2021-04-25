#include "WorkspaceVector3DotVector3.h"

WorkspaceVector3DotVector3::WorkspaceVector3DotVector3(ImTextureID headerBackground, WorkspaceVector3DotVector3Args const& args)
    : WorkspaceFloat(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVector3DotVector3::WorkspaceVector3DotVector3(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::Vector3DotVector3>(), headerLabel, nodeLabel)
{}

void WorkspaceVector3DotVector3::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}

