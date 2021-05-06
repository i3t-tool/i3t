//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatFree.h"

WorkspaceQuatFree::WorkspaceQuatFree(ImTextureID headerBackground, WorkspaceQuatFreeArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceQuatFree::WorkspaceQuatFree(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::Vector4>(), headerLabel, nodeLabel)
{}

void WorkspaceQuatFree::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}

