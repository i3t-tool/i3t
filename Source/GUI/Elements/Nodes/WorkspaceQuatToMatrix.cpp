//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceQuatToMatrix.h"

WorkspaceQuatToMatrix::WorkspaceQuatToMatrix(ImTextureID headerBackground, WorkspaceQuatToMatrixArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceQuatToMatrix::WorkspaceQuatToMatrix(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::QuatToMatrix>(), headerLabel, nodeLabel){
}

void WorkspaceQuatToMatrix::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}

