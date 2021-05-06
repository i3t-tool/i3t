//
// Created by Sofie on 28.04.2021.
//
#include "WorkspaceFloatPowFloat.h"

WorkspaceFloatPowFloat::WorkspaceFloatPowFloat(ImTextureID headerBackground, WorkspaceFloatPowFloatArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceFloatPowFloat::WorkspaceFloatPowFloat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::FloatPowFloat>(), headerLabel, nodeLabel)
{}

void WorkspaceFloatPowFloat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
