#include "WorkspaceVectorSubVector.h"

WorkspaceVectorSubVector::WorkspaceVectorSubVector(ImTextureID headerBackground, WorkspaceVectorSubVectorArgs const& args)
    : WorkspaceVector4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVectorSubVector::WorkspaceVectorSubVector(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector4(headerBackground, Core::Builder::createNode<ENodeType::VectorSubVector>(), headerLabel, nodeLabel)
{}

void WorkspaceVectorSubVector::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}

