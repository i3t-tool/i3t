#include "WorkspaceMakeLookAt.h"
WorkspaceMakeLookAt::WorkspaceMakeLookAt()
    :   WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::MakeLookAt>())
{}

