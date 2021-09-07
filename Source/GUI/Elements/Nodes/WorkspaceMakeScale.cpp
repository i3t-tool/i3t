//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceMakeScale.h"

WorkspaceMakeScale::WorkspaceMakeScale()
    :   WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::MakeScale>())
{}

