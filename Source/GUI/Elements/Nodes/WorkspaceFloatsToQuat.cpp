//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceFloatsToQuat.h"

WorkspaceFloatsToQuat::WorkspaceFloatsToQuat(ImTextureID headerBackground, WorkspaceFloatsToQuatArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceFloatsToQuat::WorkspaceFloatsToQuat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::FloatsToQuat>(), headerLabel, nodeLabel)
{}

void WorkspaceFloatsToQuat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
