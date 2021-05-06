//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceMatrixToFloats.h"

WorkspaceMatrixToFloats::WorkspaceMatrixToFloats(ImTextureID headerBackground, WorkspaceMatrixToFloatsArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMatrixToFloats::WorkspaceMatrixToFloats(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::MatrixToFloats>(), headerLabel, nodeLabel)
{}

void WorkspaceMatrixToFloats::drawDataSetValues(util::NodeBuilder& builder)
{
  //todo
  drawDataFull(builder,0);
}