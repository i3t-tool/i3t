#include "WorkspaceMatrixRotate.h"

WorkspaceMatrixRotate::WorkspaceMatrixRotate(ImTextureID headerBackground, WorkspaceMatrixRotateArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMatrixRotate::WorkspaceMatrixRotate(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::MakeAxisAngle>(), headerLabel, nodeLabel)
{}

void WorkspaceMatrixRotate::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}