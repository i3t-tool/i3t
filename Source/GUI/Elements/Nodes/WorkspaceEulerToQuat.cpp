//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceEulerToQuat.h"

WorkspaceEulerToQuat::WorkspaceEulerToQuat(ImTextureID headerBackground, WorkspaceEulerToQuatArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceEulerToQuat::WorkspaceEulerToQuat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::EulerToQuat>(), headerLabel, nodeLabel)
{}

void WorkspaceEulerToQuat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
