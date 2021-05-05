//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceQuatRotArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Quat header";
  std::string nodeLabel = "default Quat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createTransform<ENodeType::QuatRot>();
};

class WorkspaceQuatRot : public WorkspaceQuat
{
public:
  WorkspaceQuatRot(ImTextureID headerBackground, WorkspaceQuatRotArgs const& args);
  WorkspaceQuatRot(ImTextureID headerBackground, std::string headerLabel = "Quat", std::string nodeLabel = "Quat");

  void drawDataSetValues(util::NodeBuilder& builder);

};

