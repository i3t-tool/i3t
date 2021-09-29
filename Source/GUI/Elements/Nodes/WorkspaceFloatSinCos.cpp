//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceFloatSinCos.h"

WorkspaceFloatSinCos::WorkspaceFloatSinCos()
    : WorkspaceFloat(Core::Builder::createNode<ENodeType::FloatSinCos>())
{}

