#include "WorkspaceMatrixTranspose.h"

WorkspaceMatrixTranspose::WorkspaceMatrixTranspose(ImTextureID headerBackground, WorkspaceMatrixTransposeArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMatrixTranspose::WorkspaceMatrixTranspose(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Builder::createNode<ENodeType::Transpose>(), headerLabel, nodeLabel) {
}

void WorkspaceMatrixTranspose::drawData(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}
