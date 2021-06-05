//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceVecVecToQuat.h"

WorkspaceVecVecToQuat::WorkspaceVecVecToQuat(ImTextureID headerBackground, WorkspaceVecVecToQuatArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceVecVecToQuat::WorkspaceVecVecToQuat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::VecVecToQuat>(), headerLabel, nodeLabel)
{}

void WorkspaceVecVecToQuat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
