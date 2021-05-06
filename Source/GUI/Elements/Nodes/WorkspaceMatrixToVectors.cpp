//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceMatrixToVectors.h"

WorkspaceMatrixToVectors::WorkspaceMatrixToVectors(ImTextureID headerBackground, WorkspaceMatrixToVectorsArgs const& args)
    : WorkspaceVector4(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMatrixToVectors::WorkspaceMatrixToVectors(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector4(headerBackground, Core::Builder::createNode<ENodeType::MatrixToVectors>(), headerLabel, nodeLabel)
{}

void WorkspaceMatrixToVectors::drawDataSetValues(util::NodeBuilder& builder)
{
  //todo
  drawDataFull(builder,0);
}