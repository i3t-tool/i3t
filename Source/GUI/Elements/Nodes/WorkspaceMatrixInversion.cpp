#include "WorkspaceMatrixInversion.h"

WorkspaceMatrixInversion::WorkspaceMatrixInversion(ImTextureID headerBackground, WorkspaceMatrixInversionArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMatrixInversion::WorkspaceMatrixInversion(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::Inversion>(), headerLabel, nodeLabel)
{}

void WorkspaceMatrixInversion::drawDataSetValues(util::NodeBuilder& builder)
{
    //TODO SS
	drawDataFull(builder);

}

