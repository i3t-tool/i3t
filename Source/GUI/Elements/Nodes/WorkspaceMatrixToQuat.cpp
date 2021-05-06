//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceMatrixToQuat.h"

WorkspaceMatrixToQuat::WorkspaceMatrixToQuat(ImTextureID headerBackground, WorkspaceMatrixToQuatArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceMatrixToQuat::WorkspaceMatrixToQuat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createNode<ENodeType::MatrixToQuat>(), headerLabel, nodeLabel)
{}

void WorkspaceMatrixToQuat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
