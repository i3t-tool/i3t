#include "WorkspaceMatrixInversion.h"
WorkspaceMatrixInversion::WorkspaceMatrixInversion()
    : WorkspaceMatrix4x4(Core::Builder::createNode<ENodeType::Inversion>())
{}
