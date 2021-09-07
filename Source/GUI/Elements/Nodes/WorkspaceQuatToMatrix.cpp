//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceQuatToMatrix.h"

WorkspaceQuatToMatrix::WorkspaceQuatToMatrix()
    : WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::QuatToMatrix>())
{}


