//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceNormalizeQuatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Normalize Quat header";
  std::string nodeLabel = "default Normalize Quat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::NormalizeQuat>();
};

class WorkspaceNormalizeQuat : public WorkspaceQuat
{
public:
  WorkspaceNormalizeQuat(ImTextureID headerBackground, WorkspaceNormalizeQuatArgs const& args);
  WorkspaceNormalizeQuat(ImTextureID headerBackground, std::string headerLabel = "Normalize Quat", std::string nodeLabel = "Normalize Quat");

  void drawDataSetValues(util::NodeBuilder& builder);

};

