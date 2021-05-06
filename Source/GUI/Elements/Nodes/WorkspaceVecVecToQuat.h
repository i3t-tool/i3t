//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceVecVecToQuatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default quat(vec3, vec3) header";
  std::string nodeLabel = "default quat(vec3, vec3) label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VecVecToQuat>();
};

class WorkspaceVecVecToQuat : public WorkspaceQuat
{
public:
  WorkspaceVecVecToQuat(ImTextureID headerBackground, WorkspaceVecVecToQuatArgs const& args);
  WorkspaceVecVecToQuat(ImTextureID headerBackground, std::string headerLabel = "quat(vec3, vec3)", std::string nodeLabel = "quat(vec3, vec3)");

  void drawDataSetValues(util::NodeBuilder& builder);

};

