//
// Created by Sofie on 29.04.2021.
//

#include "WorkspaceLookAt.h"

WorkspaceLookAt::WorkspaceLookAt()
    :   WorkspaceMatrix4x4(Core::Builder::createTransform<Core::LookAt>())
{}

