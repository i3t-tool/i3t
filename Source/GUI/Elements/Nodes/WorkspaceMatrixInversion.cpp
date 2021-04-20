#include "WorkspaceMatrixInversion.h"

WorkspaceMatrixInversion::WorkspaceMatrixInversion(ImTextureID headerBackground, WorkspaceMatrixInversionArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {  .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMatrixInversion::WorkspaceMatrixInversion(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Builder::createNode<ENodeType::Inversion>(), headerLabel, nodeLabel)
{}


void WorkspaceMatrixInversion::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataLabel(builder);
}
