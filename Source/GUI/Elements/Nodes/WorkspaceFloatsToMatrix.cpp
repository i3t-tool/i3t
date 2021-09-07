//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceFloatsToMatrix.h"

WorkspaceFloatsToMatrix::WorkspaceFloatsToMatrix()
    : WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::FloatsToMatrix>())
{}

