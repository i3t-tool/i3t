//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatFree.h"

WorkspaceQuatFree::WorkspaceQuatFree()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::QuatToQuat>())
{}


