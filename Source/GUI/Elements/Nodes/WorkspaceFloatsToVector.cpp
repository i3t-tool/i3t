//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceFloatsToVector.h"

WorkspaceFloatsToVector::WorkspaceFloatsToVector()
    : WorkspaceVector4(Core::Builder::createNode<ENodeType::FloatsToVector>())
{}


