//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceFloatVec3.h"

struct WorkspaceQuatToAngleAxisArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default quat -> angle, axis header";
  std::string nodeLabel = "quat -> angle, axis";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatToAngleAxis>();
};


class WorkspaceQuatToAngleAxis : public WorkspaceFloatVec3
{
public:
  WorkspaceQuatToAngleAxis(ImTextureID headerBackground, WorkspaceQuatToAngleAxisArgs const& args);
  WorkspaceQuatToAngleAxis(ImTextureID headerBackground, std::string headerLabel = "quat -> angle, axis", std::string nodeLabel = "quat -> angle, axis");

  void drawDataSetValues(util::NodeBuilder& builder);

};

