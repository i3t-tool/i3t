//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatMulQuat.h"

WorkspaceQuatMulQuat::WorkspaceQuatMulQuat(ImTextureID headerBackground, WorkspaceQuatMulQuatArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceQuatMulQuat::WorkspaceQuatMulQuat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::QuatMulQuat>(), headerLabel, nodeLabel)
{}

void WorkspaceQuatMulQuat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
