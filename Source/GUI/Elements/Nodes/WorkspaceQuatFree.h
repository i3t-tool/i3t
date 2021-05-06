//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceQuatFreeArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Quat header";
  std::string nodeLabel = "default Quat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector4>();
};

class WorkspaceQuatFree : public WorkspaceQuat
{
public:
  WorkspaceQuatFree(ImTextureID headerBackground, WorkspaceQuatFreeArgs const& args);
  WorkspaceQuatFree(ImTextureID headerBackground, std::string headerLabel = "Quat", std::string nodeLabel = "Quat");

  void drawDataSetValues(util::NodeBuilder& builder);

};

