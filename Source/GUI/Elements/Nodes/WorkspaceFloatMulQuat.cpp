//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceFloatMulQuat.h"

WorkspaceFloatMulQuat::WorkspaceFloatMulQuat(ImTextureID headerBackground, WorkspaceFloatMulQuatArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceFloatMulQuat::WorkspaceFloatMulQuat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::FloatMulQuat>(), headerLabel, nodeLabel)
{}

void WorkspaceFloatMulQuat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
