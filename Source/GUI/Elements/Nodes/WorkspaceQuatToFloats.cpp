//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatToFloats.h"

WorkspaceQuatToFloats::WorkspaceQuatToFloats(ImTextureID headerBackground, WorkspaceQuatToFloatsArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceQuatToFloats::WorkspaceQuatToFloats(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::QuatToFloats>(), headerLabel, nodeLabel)
{}

void WorkspaceQuatToFloats::drawDataSetValues(util::NodeBuilder& builder)
{
  //todo
  drawDataFull(builder,0);
}