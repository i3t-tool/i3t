//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatInverse.h"

WorkspaceQuatInverse::WorkspaceQuatInverse()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::QuatInverse>())
{}
