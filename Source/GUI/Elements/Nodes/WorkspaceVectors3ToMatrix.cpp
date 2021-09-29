//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceVectors3ToMatrix.h"

WorkspaceVectors3ToMatrix::WorkspaceVectors3ToMatrix()
    : WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::Vectors3ToMatrix>())
{}


