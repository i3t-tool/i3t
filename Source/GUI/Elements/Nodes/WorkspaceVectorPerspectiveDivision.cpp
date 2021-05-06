#include "WorkspaceVectorPerspectiveDivision.h"

WorkspaceVectorPerspectiveDivision::WorkspaceVectorPerspectiveDivision(ImTextureID headerBackground, WorkspaceVectorPerspectiveDivisionArgs const& args)
    : WorkspaceVector4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVectorPerspectiveDivision::WorkspaceVectorPerspectiveDivision(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector4(headerBackground, Core::Builder::createNode<ENodeType::VectorPerspectiveDivision>(), headerLabel, nodeLabel)
{}

void WorkspaceVectorPerspectiveDivision::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder,0);
}

