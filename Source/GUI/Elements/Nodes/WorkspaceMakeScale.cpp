//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceMakeScale.h"

WorkspaceMakeScale::WorkspaceMakeScale(ImTextureID headerBackground, WorkspaceMakeScaleArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMakeScale::WorkspaceMakeScale(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::MakeScale>(), headerLabel, nodeLabel)
{}

void WorkspaceMakeScale::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}