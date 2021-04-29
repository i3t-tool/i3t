//
// Created by Sofie on 29.04.2021.
//

#include "WorkspaceLookAt.h"

WorkspaceLookAt::WorkspaceLookAt(ImTextureID headerBackground, WorkspaceLookAtArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createTransform<Core::Translation>() })
{}

WorkspaceLookAt::WorkspaceLookAt(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createTransform<Core::LookAt>(), headerLabel, nodeLabel)
{}

void WorkspaceLookAt::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder);
}
