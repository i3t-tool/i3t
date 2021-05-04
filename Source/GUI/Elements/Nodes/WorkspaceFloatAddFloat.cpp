//
// Created by Sofie on 28.04.2021.
//
#include "WorkspaceFloatAddFloat.h"

WorkspaceFloatAddFloat::WorkspaceFloatAddFloat(ImTextureID headerBackground, WorkspaceFloatAddFloatArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceFloatAddFloat::WorkspaceFloatAddFloat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::FloatAddFloat>(), headerLabel, nodeLabel)
{}

void WorkspaceFloatAddFloat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}
