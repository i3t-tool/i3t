//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceMatrixToVectors.h"

WorkspaceMatrixToVectors::WorkspaceMatrixToVectors()
    : WorkspaceVector4(Core::Builder::createNode<ENodeType::MatrixToVectors>())
{}

