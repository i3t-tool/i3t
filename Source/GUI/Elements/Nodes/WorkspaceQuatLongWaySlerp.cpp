//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatLongWaySlerp.h"

WorkspaceQuatLongWaySlerp::WorkspaceQuatLongWaySlerp(ImTextureID headerBackground, WorkspaceQuatLongWaySlerpArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceQuatLongWaySlerp::WorkspaceQuatLongWaySlerp(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::QuatLongWaySlerp>(), headerLabel, nodeLabel)
{}

void WorkspaceQuatLongWaySlerp::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
