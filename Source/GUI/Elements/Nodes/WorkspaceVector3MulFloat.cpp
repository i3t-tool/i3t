#include "WorkspaceVector3MulFloat.h"

WorkspaceVector3MulFloat::WorkspaceVector3MulFloat(ImTextureID headerBackground, WorkspaceVector3MulFloatArgs const& args)
    : WorkspaceVector3(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVector3MulFloat::WorkspaceVector3MulFloat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector3(headerBackground, Core::Builder::createNode<ENodeType::Vector3MulFloat>(), headerLabel, nodeLabel)
{}

void WorkspaceVector3MulFloat::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}

