#include "WorkspaceMatrixMulFloat.h"

WorkspaceMatrixMulFloat::WorkspaceMatrixMulFloat(ImTextureID headerBackground, WorkspaceMatrixMulFloatArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMatrixMulFloat::WorkspaceMatrixMulFloat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::MatrixMulFloat>(), headerLabel, nodeLabel)
{}

void WorkspaceMatrixMulFloat::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataLabel(builder);
}
