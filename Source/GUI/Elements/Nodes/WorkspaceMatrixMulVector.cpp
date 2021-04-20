#include "WorkspaceMatrixMulVector.h"

WorkspaceMatrixMulVector::WorkspaceMatrixMulVector(ImTextureID headerBackground, WorkspaceMatrixMulVectorArgs const& args)
    : WorkspaceVector4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMatrixMulVector::WorkspaceMatrixMulVector(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector4(headerBackground, Builder::createNode<ENodeType::MatrixMulVector>(), headerLabel, nodeLabel)
{}

void WorkspaceMatrixMulVector::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}
