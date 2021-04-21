#include "WorkspaceMatrixAddMatrix.h"

WorkspaceMatrixAddMatrix::WorkspaceMatrixAddMatrix(ImTextureID headerBackground, WorkspaceMatrixAddMatrixArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMatrixAddMatrix::WorkspaceMatrixAddMatrix(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::MatrixAddMatrix>(), headerLabel, nodeLabel)
{}

void WorkspaceMatrixAddMatrix::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataLabel(builder);
}
