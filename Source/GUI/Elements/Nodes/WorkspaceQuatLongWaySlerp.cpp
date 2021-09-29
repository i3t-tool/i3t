//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatLongWaySlerp.h"

WorkspaceQuatLongWaySlerp::WorkspaceQuatLongWaySlerp()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::QuatLongWaySlerp>())
{}
