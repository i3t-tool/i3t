//
// Created by Sofie on 05.05.2021.
//
#include "WorkspaceMatrixToFloats.h"

WorkspaceMatrixToFloats::WorkspaceMatrixToFloats()
    : WorkspaceFloat(Core::Builder::createNode<ENodeType::MatrixToFloats>())
{}
