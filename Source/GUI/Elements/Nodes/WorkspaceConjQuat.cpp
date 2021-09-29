//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceConjQuat.h"

WorkspaceConjQuat::WorkspaceConjQuat()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::ConjQuat>())
{}

