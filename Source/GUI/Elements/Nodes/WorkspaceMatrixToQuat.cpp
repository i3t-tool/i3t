//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceMatrixToQuat.h"

WorkspaceMatrixToQuat::WorkspaceMatrixToQuat()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::MatrixToQuat>())
{}
