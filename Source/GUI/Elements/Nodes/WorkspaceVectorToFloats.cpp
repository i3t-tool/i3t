//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceVectorToFloats.h"

WorkspaceVectorToFloats::WorkspaceVectorToFloats(ImTextureID headerBackground, WorkspaceVectorToFloatsArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVectorToFloats::WorkspaceVectorToFloats(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::VectorToFloats>(), headerLabel, nodeLabel)
{}

void WorkspaceVectorToFloats::drawDataSetValues(util::NodeBuilder& builder)
{
  //todo
  drawDataFull(builder,0);
}

