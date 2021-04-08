#include "WorkspaceDeterminant.h"

WorkspaceDeterminant::WorkspaceDeterminant(ImTextureID headerBackground, WorkspaceDeterminantArgs const& args)
    : WorkspaceFloat(headerBackground, { .viewScale = args.viewScale, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceDeterminant::WorkspaceDeterminant(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Builder::createNode<ENodeType::Determinant>(), headerLabel, nodeLabel)
{}

void WorkspaceDeterminant::drawData(util::NodeBuilder& builder)
{
    drawDataFull(builder); 
}