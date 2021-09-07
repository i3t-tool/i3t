//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceFloatsToQuat.h"

WorkspaceFloatsToQuat::WorkspaceFloatsToQuat()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::FloatsToQuat>())
{}

