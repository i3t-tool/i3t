//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceVector3ToFloats.h"

WorkspaceVector3ToFloats::WorkspaceVector3ToFloats()
    : WorkspaceFloat(Core::Builder::createNode<ENodeType::Vector3ToFloats>())
{}
