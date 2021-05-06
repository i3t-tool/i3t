#include "WorkspaceVectorAddVector.h"

WorkspaceVectorAddVector::WorkspaceVectorAddVector(ImTextureID headerBackground, WorkspaceVectorAddVectorArgs const& args)
    : WorkspaceVector4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVectorAddVector::WorkspaceVectorAddVector(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector4(headerBackground, Core::Builder::createNode<ENodeType::VectorAddVector>(), headerLabel, nodeLabel)
{}

void WorkspaceVectorAddVector::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder,0);
}

