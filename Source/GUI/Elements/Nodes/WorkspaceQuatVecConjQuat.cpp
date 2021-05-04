//
// Created by Sofie on 04.05.2021.
//

#include "WorkspaceQuatVecConjQuat.h"

WorkspaceQuatVecConjQuat::WorkspaceQuatVecConjQuat(ImTextureID headerBackground, WorkspaceQuatVecConjQuatArgs const& args)
    : WorkspaceVector3(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceQuatVecConjQuat::WorkspaceQuatVecConjQuat(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector3(headerBackground, Core::Builder::createNode<ENodeType::QuatVecConjQuat>(), headerLabel, nodeLabel)
{}

void WorkspaceQuatVecConjQuat::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}

