//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceFloatsToVector.h"

WorkspaceFloatsToVector::WorkspaceFloatsToVector(ImTextureID headerBackground, WorkspaceFloatsToVectorArgs const& args)
    : WorkspaceVector4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceFloatsToVector::WorkspaceFloatsToVector(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector4(headerBackground, Core::Builder::createNode<ENodeType::FloatsToVector>(), headerLabel, nodeLabel){
}

void WorkspaceFloatsToVector::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}

