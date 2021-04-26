#include "WorkspaceMixVector.h"

WorkspaceMixVector::WorkspaceMixVector(ImTextureID headerBackground, WorkspaceMixVectorArgs const& args)
    : WorkspaceVector4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMixVector::WorkspaceMixVector(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector4(headerBackground, Core::Builder::createNode<ENodeType::MixVector>(), headerLabel, nodeLabel)
{}

void WorkspaceMixVector::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}

