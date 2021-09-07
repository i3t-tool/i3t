//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceNormalizeQuat.h"

WorkspaceNormalizeQuat::WorkspaceNormalizeQuat()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::NormalizeQuat>())
{}
