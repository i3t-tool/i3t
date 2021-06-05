#include "WorkspaceMakeEulerX.h"

WorkspaceMakeEulerX::WorkspaceMakeEulerX(ImTextureID headerBackground, WorkspaceMakeEulerXArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMakeEulerX::WorkspaceMakeEulerX(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::MakeEulerX>(), headerLabel, nodeLabel)
{}

void WorkspaceMakeEulerX::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}