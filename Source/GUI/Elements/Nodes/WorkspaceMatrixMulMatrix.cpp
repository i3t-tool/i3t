#include "WorkspaceMatrixMulMatrix.h"

WorkspaceMatrixMulMatrix::WorkspaceMatrixMulMatrix(ImTextureID headerBackground, WorkspaceMatrixMulMatrixArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{
}

WorkspaceMatrixMulMatrix::WorkspaceMatrixMulMatrix(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::MatrixMulMatrix>(), headerLabel, nodeLabel)
{}
void WorkspaceMatrixMulMatrix::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataLabel(builder);
}