//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceFloatVecToQuat.h"

WorkspaceFloatVecToQuat::WorkspaceFloatVecToQuat()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::FloatVecToQuat>())
{}

