//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceAngleAxisToQuatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default quat(angle, axis) header";
  std::string nodeLabel = "default quat(angle, axis) label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::AngleAxisToQuat>();
};

class WorkspaceAngleAxisToQuat : public WorkspaceQuat
{
public:
  WorkspaceAngleAxisToQuat(ImTextureID headerBackground, WorkspaceAngleAxisToQuatArgs const& args);
  WorkspaceAngleAxisToQuat(ImTextureID headerBackground, std::string headerLabel = "quat(angle, axis)", std::string nodeLabel = "quat(angle, axis)");

  void drawDataSetValues(util::NodeBuilder& builder);

};
