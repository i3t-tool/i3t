//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceAngleAxisToQuat.h"

WorkspaceAngleAxisToQuat::WorkspaceAngleAxisToQuat(ImTextureID headerBackground, WorkspaceAngleAxisToQuatArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceAngleAxisToQuat::WorkspaceAngleAxisToQuat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::AngleAxisToQuat>(), headerLabel, nodeLabel)
{}

void WorkspaceAngleAxisToQuat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
