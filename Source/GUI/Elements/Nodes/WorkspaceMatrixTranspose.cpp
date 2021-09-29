#include "WorkspaceMatrixTranspose.h"

WorkspaceMatrixTranspose::WorkspaceMatrixTranspose()
    :   WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::Transpose>())
{}

