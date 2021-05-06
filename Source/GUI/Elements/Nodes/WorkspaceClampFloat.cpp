//
// Created by Sofie on 28.04.2021.
//
#include "WorkspaceClampFloat.h"

WorkspaceClampFloat::WorkspaceClampFloat(ImTextureID headerBackground, WorkspaceClampFloatArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceClampFloat::WorkspaceClampFloat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::ClampFloat>(), headerLabel, nodeLabel)
{}

void WorkspaceClampFloat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
