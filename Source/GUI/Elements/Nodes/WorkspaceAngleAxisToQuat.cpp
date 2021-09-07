//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceAngleAxisToQuat.h"

WorkspaceAngleAxisToQuat::WorkspaceAngleAxisToQuat()
    : WorkspaceQuat(Core::Builder::createNode<ENodeType::AngleAxisToQuat>())
{}

