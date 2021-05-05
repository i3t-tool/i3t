//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceFloatVecToQuat.h"

WorkspaceFloatVecToQuat::WorkspaceFloatVecToQuat(ImTextureID headerBackground, WorkspaceFloatVecToQuatArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceFloatVecToQuat::WorkspaceFloatVecToQuat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::FloatVecToQuat>(), headerLabel, nodeLabel)
{}

void WorkspaceFloatVecToQuat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}