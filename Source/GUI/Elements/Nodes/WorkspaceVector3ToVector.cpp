//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceVector3ToVector.h"

WorkspaceVector3ToVector::WorkspaceVector3ToVector()
    : WorkspaceVector4(Core::Builder::createNode<ENodeType::Vector3ToVector>())
{}

