//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceFloatSinCos.h"

WorkspaceFloatSinCos::WorkspaceFloatSinCos(ImTextureID headerBackground, WorkspaceFloatSinCosArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceFloatSinCos::WorkspaceFloatSinCos(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::FloatSinCos>(), headerLabel, nodeLabel)
{}

void WorkspaceFloatSinCos::drawDataSetValues(util::NodeBuilder& builder)
{
  //todo
  drawDataFull(builder,0);
}