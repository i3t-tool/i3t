#include "WorkspaceMakeAxisAngle.h"

WorkspaceMakeAxisAngle::WorkspaceMakeAxisAngle()
    :   WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::MakeAxisAngle>())
{}

