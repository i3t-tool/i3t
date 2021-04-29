//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceFloatsToVector3.h"

WorkspaceFloatsToVector3::WorkspaceFloatsToVector3(ImTextureID headerBackground, WorkspaceFloatsToVector3Args const& args)
    : WorkspaceVector3(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceFloatsToVector3::WorkspaceFloatsToVector3(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceVector3(headerBackground, Core::Builder::createNode<ENodeType::FloatsToVector3>(), headerLabel, nodeLabel)
{}

void WorkspaceFloatsToVector3::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}

