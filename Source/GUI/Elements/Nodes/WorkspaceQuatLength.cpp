//
// Created by Sofie on 04.05.2021.
//

#include "WorkspaceQuatLength.h"

WorkspaceQuatLength::WorkspaceQuatLength(ImTextureID headerBackground, WorkspaceQuatLengthArgs const& args)
    : WorkspaceFloat(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceQuatLength::WorkspaceQuatLength(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::QuatLength>(), headerLabel, nodeLabel)
{}

void WorkspaceQuatLength::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}

