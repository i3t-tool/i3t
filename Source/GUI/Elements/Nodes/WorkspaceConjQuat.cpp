//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceConjQuat.h"

WorkspaceConjQuat::WorkspaceConjQuat(ImTextureID headerBackground, WorkspaceConjQuatArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceConjQuat::WorkspaceConjQuat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::ConjQuat>(), headerLabel, nodeLabel)
{}

void WorkspaceConjQuat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
