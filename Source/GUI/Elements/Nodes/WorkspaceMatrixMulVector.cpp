#include "WorkspaceMatrixMulVector.h"
WorkspaceMatrixMulVector::WorkspaceMatrixMulVector()
    :   WorkspaceVector4(Core::Builder::createNode<ENodeType::MatrixMulVector>())
{}
