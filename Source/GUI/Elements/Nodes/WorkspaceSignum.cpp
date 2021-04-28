//
// Created by Sofie on 28.04.2021.
//
#include "WorkspaceSignum.h"

WorkspaceSignum::WorkspaceSignum(ImTextureID headerBackground, WorkspaceSignumArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceSignum::WorkspaceSignum(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::Signum>(), headerLabel, nodeLabel)
{}

void WorkspaceSignum::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}
