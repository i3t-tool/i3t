//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceTRToMatrix.h"

WorkspaceTRToMatrix::WorkspaceTRToMatrix(ImTextureID headerBackground, WorkspaceTRToMatrixArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceTRToMatrix::WorkspaceTRToMatrix(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::TRToMatrix>(), headerLabel, nodeLabel){
}

void WorkspaceTRToMatrix::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}

