#include "WorkspaceDeterminant.h"

WorkspaceDeterminant::WorkspaceDeterminant(ImTextureID headerBackground, WorkspaceDeterminantArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceDeterminant::WorkspaceDeterminant(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::Determinant>(), headerLabel, nodeLabel)
{}

void WorkspaceDeterminant::drawDataSetValues(util::NodeBuilder& builder)
{
    /* \todo JH - is something to do here? function have to exist anyway... */
    drawDataFull(builder, 0);
}


