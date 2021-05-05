#include "WorkspaceMakeAxisAngle.h"

WorkspaceMakeAxisAngle::WorkspaceMakeAxisAngle(ImTextureID headerBackground, WorkspaceMakeAxisAngleArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMakeAxisAngle::WorkspaceMakeAxisAngle(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::MakeAxisAngle>(), headerLabel, nodeLabel)
{}

void WorkspaceMakeAxisAngle::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder,0);
}