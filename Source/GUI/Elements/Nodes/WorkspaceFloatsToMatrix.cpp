//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceFloatsToMatrix.h"

WorkspaceFloatsToMatrix::WorkspaceFloatsToMatrix(ImTextureID headerBackground, WorkspaceFloatsToMatrixArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceFloatsToMatrix::WorkspaceFloatsToMatrix(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::FloatsToMatrix>(), headerLabel, nodeLabel){
}

void WorkspaceFloatsToMatrix::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}

