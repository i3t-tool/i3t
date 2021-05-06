//
// Created by Sofie on 29.04.2021.
//

#include "WorkspaceTransformationFree.h"

WorkspaceTransformationFree::WorkspaceTransformationFree(ImTextureID headerBackground, WorkspaceTransformationFreeArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createTransform<Core::Translation>() })
{}

WorkspaceTransformationFree::WorkspaceTransformationFree(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createTransform<Core::Free>(), headerLabel, nodeLabel)
{}

void WorkspaceTransformationFree::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
