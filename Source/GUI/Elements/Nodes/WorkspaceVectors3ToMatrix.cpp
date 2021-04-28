//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceVectors3ToMatrix.h"

WorkspaceVectors3ToMatrix::WorkspaceVectors3ToMatrix(ImTextureID headerBackground, WorkspaceVectors3ToMatrixArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceVectors3ToMatrix::WorkspaceVectors3ToMatrix(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::Vectors3ToMatrix>(), headerLabel, nodeLabel){
}

void WorkspaceVectors3ToMatrix::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}

