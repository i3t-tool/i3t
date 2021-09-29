#include "WorkspaceMatrixMulFloat.h"
WorkspaceMatrixMulFloat::WorkspaceMatrixMulFloat()
    :   WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::MatrixMulFloat>())
{}

