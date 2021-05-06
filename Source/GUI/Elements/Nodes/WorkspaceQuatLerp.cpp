//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatLerp.h"

WorkspaceQuatLerp::WorkspaceQuatLerp(ImTextureID headerBackground, WorkspaceQuatLerpArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceQuatLerp::WorkspaceQuatLerp(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::QuatLerp>(), headerLabel, nodeLabel)
{}

void WorkspaceQuatLerp::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
