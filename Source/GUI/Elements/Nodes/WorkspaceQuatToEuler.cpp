//
// Created by Sofie on 04.05.2021.
//

#include "WorkspaceQuatToEuler.h"

WorkspaceQuatToEuler::WorkspaceQuatToEuler()
    : WorkspaceFloat(Core::Builder::createNode<ENodeType::QuatToEuler>())
{}
