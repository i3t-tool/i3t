//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceFloatMulQuat.h"

WorkspaceFloatMulQuat::WorkspaceFloatMulQuat()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::FloatMulQuat>())
{}

