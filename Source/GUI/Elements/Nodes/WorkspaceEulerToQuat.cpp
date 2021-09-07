//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceEulerToQuat.h"

WorkspaceEulerToQuat::WorkspaceEulerToQuat()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::EulerToQuat>())
{}

