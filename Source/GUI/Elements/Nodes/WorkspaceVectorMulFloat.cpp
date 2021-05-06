#include "WorkspaceVectorMulFloat.h"

WorkspaceVectorMulFloat::WorkspaceVectorMulFloat(ImTextureID headerBackground, WorkspaceVectorMulFloatArgs const& args)
    : WorkspaceVector4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVectorMulFloat::WorkspaceVectorMulFloat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector4(headerBackground, Core::Builder::createNode<ENodeType::VectorMulFloat>(), headerLabel, nodeLabel)
{}

void WorkspaceVectorMulFloat::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder,0);
}

