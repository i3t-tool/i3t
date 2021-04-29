//
// Created by Sofie on 28.04.2021.
//

#include "WorkspaceMakeTranslation.h"

WorkspaceMakeTranslation::WorkspaceMakeTranslation(ImTextureID headerBackground, WorkspaceMakeTranslationArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{}

WorkspaceMakeTranslation::WorkspaceMakeTranslation(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::MakeTranslation>(), headerLabel, nodeLabel)
{}

void WorkspaceMakeTranslation::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}