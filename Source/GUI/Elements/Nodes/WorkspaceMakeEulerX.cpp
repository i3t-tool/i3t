#include "WorkspaceMakeEulerX.h"
WorkspaceMakeEulerX::WorkspaceMakeEulerX()
    :   WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::MakeEulerX>())
{}

