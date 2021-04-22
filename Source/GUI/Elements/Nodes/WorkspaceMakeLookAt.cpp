#include "WorkspaceMakeLookAt.h"

WorkspaceMakeLookAt::WorkspaceMakeLookAt(ImTextureID headerBackground, WorkspaceMakeLookAtArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMakeLookAt::WorkspaceMakeLookAt(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::MakeLookAt>(), headerLabel, nodeLabel)
{}

void WorkspaceMakeLookAt::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}