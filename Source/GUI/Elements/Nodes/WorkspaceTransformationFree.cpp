//
// Created by Sofie on 29.04.2021.
//

#include "WorkspaceTransformationFree.h"


WorkspaceTransformationFree::WorkspaceTransformationFree()
    :   WorkspaceMatrix4x4(Core::Builder::createTransform<Core::Free>())
{}

