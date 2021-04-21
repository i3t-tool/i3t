#include "WorkspaceMakeFrustum.h"

WorkspaceMakeFrustum::WorkspaceMakeFrustum(ImTextureID headerBackground, WorkspaceMakeFrustumArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMakeFrustum::WorkspaceMakeFrustum(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::MakeFrustum>(), headerLabel, nodeLabel)
{}

void WorkspaceMakeFrustum::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}