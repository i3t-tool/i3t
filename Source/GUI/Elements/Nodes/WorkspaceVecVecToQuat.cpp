//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceVecVecToQuat.h"

WorkspaceVecVecToQuat::WorkspaceVecVecToQuat()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::VecVecToQuat>())
{}

