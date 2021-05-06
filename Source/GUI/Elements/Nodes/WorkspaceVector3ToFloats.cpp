//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceVector3ToFloats.h"

WorkspaceVector3ToFloats::WorkspaceVector3ToFloats(ImTextureID headerBackground, WorkspaceVector3ToFloatsArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVector3ToFloats::WorkspaceVector3ToFloats(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::Vector3ToFloats>(), headerLabel, nodeLabel)
{}

void WorkspaceVector3ToFloats::drawDataSetValues(util::NodeBuilder& builder)
{
  //todo
  drawDataFull(builder,0);
}