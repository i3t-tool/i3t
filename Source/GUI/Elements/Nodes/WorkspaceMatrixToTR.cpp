//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceMatrixToTR.h"

WorkspaceMatrixToTR::WorkspaceMatrixToTR(ImTextureID headerBackground, WorkspaceMatrixToTRArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMatrixToTR::WorkspaceMatrixToTR(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::MatrixToTR>(), headerLabel, nodeLabel)
{}

void WorkspaceMatrixToTR::drawDataSetValues(util::NodeBuilder& builder)
{
  //todo
  drawDataFull(builder,0);
}