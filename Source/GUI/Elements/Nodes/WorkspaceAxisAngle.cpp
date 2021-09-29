//
// Created by Sofie on 29.04.2021.
//

#include "WorkspaceAxisAngle.h"

WorkspaceAxisAngle::WorkspaceAxisAngle()
    :   WorkspaceMatrix4x4(Core::Builder::createTransform<Core::AxisAngleRot>())
{}
