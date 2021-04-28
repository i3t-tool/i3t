//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceVectorsToMatrix.h"

WorkspaceVectorsToMatrix::WorkspaceVectorsToMatrix(ImTextureID headerBackground, WorkspaceVectorsToMatrixArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceVectorsToMatrix::WorkspaceVectorsToMatrix(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::VectorsToMatrix>(), headerLabel, nodeLabel){
}

void WorkspaceVectorsToMatrix::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}

