//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceMatrixToTR.h"

WorkspaceMatrixToTR::WorkspaceMatrixToTR()
    : WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::MatrixToTR>())
{}
