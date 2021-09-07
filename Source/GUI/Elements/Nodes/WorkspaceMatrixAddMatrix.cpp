#include "WorkspaceMatrixAddMatrix.h"
WorkspaceMatrixAddMatrix::WorkspaceMatrixAddMatrix()
    :   WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::MatrixAddMatrix>())
{}
