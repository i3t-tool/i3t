#include "WorkspaceVectorDotVector.h"

WorkspaceVectorDotVector::WorkspaceVectorDotVector(ImTextureID headerBackground, WorkspaceVectorDotVectorArgs const& args)
    : WorkspaceFloat(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVectorDotVector::WorkspaceVectorDotVector(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::VectorDotVector>(), headerLabel, nodeLabel)
{}

void WorkspaceVectorDotVector::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder,0);
}

