//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceNormalizeQuat.h"

WorkspaceNormalizeQuat::WorkspaceNormalizeQuat(ImTextureID headerBackground, WorkspaceNormalizeQuatArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceNormalizeQuat::WorkspaceNormalizeQuat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::NormalizeQuat>(), headerLabel, nodeLabel)
{}

void WorkspaceNormalizeQuat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
