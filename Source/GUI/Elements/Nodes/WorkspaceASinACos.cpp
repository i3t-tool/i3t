//
// Created by Sofie on 05.05.2021.
//
#include "WorkspaceASinACos.h"

WorkspaceASinACos::WorkspaceASinACos(ImTextureID headerBackground, WorkspaceASinACosArgs const& args)
    : WorkspaceFloat(headerBackground, {.levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceASinACos::WorkspaceASinACos(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceFloat(headerBackground, Core::Builder::createNode<ENodeType::ASinACos>(), headerLabel, nodeLabel)
{}

void WorkspaceASinACos::drawDataSetValues(util::NodeBuilder& builder)
{
  //todo
  drawDataFull(builder,0);
}