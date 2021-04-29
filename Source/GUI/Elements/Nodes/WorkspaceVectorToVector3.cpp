//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceVectorToVector3.h"

WorkspaceVectorToVector3::WorkspaceVectorToVector3(ImTextureID headerBackground, WorkspaceVectorToVector3Args const& args)
    : WorkspaceVector3(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceVectorToVector3::WorkspaceVectorToVector3(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector3(headerBackground, Core::Builder::createNode<ENodeType::VectorToVector3>(), headerLabel, nodeLabel)
{}

void WorkspaceVectorToVector3::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}

