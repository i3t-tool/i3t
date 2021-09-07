#include "WorkspaceMatrixFree.h"
WorkspaceMatrixFree::WorkspaceMatrixFree()
    :   WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::Matrix>())
{}

