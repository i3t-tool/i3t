//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceVectorsToMatrix.h"

WorkspaceVectorsToMatrix::WorkspaceVectorsToMatrix()
    : WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::VectorsToMatrix>())
{}


