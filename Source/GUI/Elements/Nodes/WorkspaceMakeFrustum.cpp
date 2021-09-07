#include "WorkspaceMakeFrustum.h"

WorkspaceMakeFrustum::WorkspaceMakeFrustum()
    :   WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::MakeFrustum>())
{}
