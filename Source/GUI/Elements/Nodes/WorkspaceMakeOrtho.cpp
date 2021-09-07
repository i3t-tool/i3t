#include "WorkspaceMakeOrtho.h"

WorkspaceMakeOrtho::WorkspaceMakeOrtho()
    :   WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::MakeOrtho>())
{}
