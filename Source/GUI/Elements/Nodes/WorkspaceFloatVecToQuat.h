//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceFloatVecToQuatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default quat(float, vec3) header";
  std::string nodeLabel = "default quat(float, vec3) label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatVecToQuat>();
};

class WorkspaceFloatVecToQuat : public WorkspaceQuat
{
public:
  WorkspaceFloatVecToQuat(ImTextureID headerBackground, WorkspaceFloatVecToQuatArgs const& args);
  WorkspaceFloatVecToQuat(ImTextureID headerBackground, std::string headerLabel = "quat(float, vec3)", std::string nodeLabel = "quat(float, vec3)");

  void drawDataSetValues(util::NodeBuilder& builder);

};
