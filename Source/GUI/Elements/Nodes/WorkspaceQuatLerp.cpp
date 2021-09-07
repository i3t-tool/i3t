//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatLerp.h"

WorkspaceQuatLerp::WorkspaceQuatLerp()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::QuatLerp>())
{}

