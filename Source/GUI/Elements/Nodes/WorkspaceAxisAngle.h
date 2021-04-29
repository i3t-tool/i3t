//
// Created by Sofie on 29.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceAxisAngleArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default AxisAngle header";
  std::string nodeLabel = "AxisAngle";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createTransform<Core::AxisAngleRot>();
};

class WorkspaceAxisAngle : public WorkspaceMatrix4x4
{
public:
  WorkspaceAxisAngle(ImTextureID headerBackground, WorkspaceAxisAngleArgs const& args);
  WorkspaceAxisAngle(ImTextureID headerBackground, std::string headerLabel = "AxisAngle", std::string nodeLabel = "AxisAngle");

  void drawDataSetValues(util::NodeBuilder& builder);
};
