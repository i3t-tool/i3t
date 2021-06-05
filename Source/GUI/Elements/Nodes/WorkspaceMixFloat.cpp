//
// Created by Sofie on 28.04.2021.
//
#include "WorkspaceMixFloat.h"

WorkspaceMixFloat::WorkspaceMixFloat(ImTextureID headerBackground, WorkspaceMixFloatArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMixFloat::WorkspaceMixFloat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::MixFloat>(), headerLabel, nodeLabel)
{}

void WorkspaceMixFloat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
