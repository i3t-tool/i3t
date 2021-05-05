//
// Created by Sofie on 29.04.2021.
//

#include "WorkspaceAxisAngle.h"

WorkspaceAxisAngle::WorkspaceAxisAngle(ImTextureID headerBackground, WorkspaceAxisAngleArgs const& args)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createTransform<Core::Translation>() })
{}

WorkspaceAxisAngle::WorkspaceAxisAngle(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createTransform<Core::AxisAngleRot>(), headerLabel, nodeLabel)
{}

void WorkspaceAxisAngle::drawDataSetValues(util::NodeBuilder& builder)
{
  drawDataFull(builder,0);
}
