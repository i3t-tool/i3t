//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatSlerp.h"

WorkspaceQuatSlerp::WorkspaceQuatSlerp()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::QuatSlerp>())
{}

