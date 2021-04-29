//
// Created by Sofie on 28.04.2021.
//
#include "WorkspaceFloatMulFloat.h"

WorkspaceFloatMulFloat::WorkspaceFloatMulFloat(ImTextureID headerBackground, WorkspaceFloatMulFloatArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceFloatMulFloat::WorkspaceFloatMulFloat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::FloatMulFloat>(), headerLabel, nodeLabel)
{}

void WorkspaceFloatMulFloat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}
