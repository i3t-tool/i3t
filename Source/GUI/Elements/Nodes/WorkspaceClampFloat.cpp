//
// Created by Sofie on 28.04.2021.
//
#include "WorkspaceClampFloat.h"

WorkspaceClampFloat::WorkspaceClampFloat()
    : WorkspaceFloat(Core::Builder::createNode<ENodeType::ClampFloat>())
{}

