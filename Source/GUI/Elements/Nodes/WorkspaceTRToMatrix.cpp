//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceTRToMatrix.h"

WorkspaceTRToMatrix::WorkspaceTRToMatrix()
    : WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::TRToMatrix>())
{}


