#include "WorkspaceVectorMulMatrix.h"

WorkspaceVectorMulMatrix::WorkspaceVectorMulMatrix(ImTextureID headerBackground, WorkspaceVectorMulMatrixArgs const& args)
    : WorkspaceVector4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVectorMulMatrix::WorkspaceVectorMulMatrix(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector4(headerBackground, Core::Builder::createNode<ENodeType::VectorMulMatrix>(), headerLabel, nodeLabel)
{}

void WorkspaceVectorMulMatrix::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}
