//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatSlerp.h"

WorkspaceQuatSlerp::WorkspaceQuatSlerp(ImTextureID headerBackground, WorkspaceQuatSlerpArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceQuatSlerp::WorkspaceQuatSlerp(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::QuatSlerp>(), headerLabel, nodeLabel)
{}

void WorkspaceQuatSlerp::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
