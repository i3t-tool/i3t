//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatMulQuat.h"

WorkspaceQuatMulQuat::WorkspaceQuatMulQuat()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::QuatMulQuat>())
{}

