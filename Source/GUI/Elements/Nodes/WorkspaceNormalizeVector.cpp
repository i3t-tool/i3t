#include "WorkspaceNormalizeVector.h"

WorkspaceNormalizeVector::WorkspaceNormalizeVector()
    : WorkspaceVector4(Core::Builder::createNode<ENodeType::NormalizeVector>())
{}
