//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceVector3ToVector.h"

WorkspaceVector3ToVector::WorkspaceVector3ToVector(ImTextureID headerBackground, WorkspaceVector3ToVectorArgs const& args)
    : WorkspaceVector4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceVector3ToVector::WorkspaceVector3ToVector(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector4(headerBackground, Core::Builder::createNode<ENodeType::Vector3ToVector>(), headerLabel, nodeLabel)
{}

void WorkspaceVector3ToVector::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}

