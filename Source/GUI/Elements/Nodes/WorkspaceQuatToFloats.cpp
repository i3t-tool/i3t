//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatToFloats.h"

WorkspaceQuatToFloats::WorkspaceQuatToFloats()
    : WorkspaceFloat(Core::Builder::createNode<ENodeType::QuatToFloats>())
{}

