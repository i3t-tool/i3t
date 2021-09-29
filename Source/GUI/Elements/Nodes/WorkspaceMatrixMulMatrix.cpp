#include "WorkspaceMatrixMulMatrix.h"
WorkspaceMatrixMulMatrix::WorkspaceMatrixMulMatrix()
    : WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::MatrixMulMatrix>())
{}
