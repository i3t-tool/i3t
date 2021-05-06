#include "WorkspaceNormalizeVector.h"

WorkspaceNormalizeVector::WorkspaceNormalizeVector(ImTextureID headerBackground, WorkspaceNormalizeVectorArgs const& args)
    : WorkspaceVector4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceNormalizeVector::WorkspaceNormalizeVector(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector4(headerBackground, Core::Builder::createNode<ENodeType::NormalizeVector>(), headerLabel, nodeLabel)
{}

void WorkspaceNormalizeVector::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder,0);
}
