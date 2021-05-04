//
// Created by Sofie on 28.04.2021.
//
#include "WorkspaceFloatDivFloat.h"

WorkspaceFloatDivFloat::WorkspaceFloatDivFloat(ImTextureID headerBackground, WorkspaceFloatDivFloatArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceFloatDivFloat::WorkspaceFloatDivFloat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::FloatDivFloat>(), headerLabel, nodeLabel)
{}

void WorkspaceFloatDivFloat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}
