//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceQuatInverseArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default inverse quat header";
  std::string nodeLabel = "default inverse quat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatInverse>();
};

class WorkspaceQuatInverse : public WorkspaceQuat
{
public:
  WorkspaceQuatInverse(ImTextureID headerBackground, WorkspaceQuatInverseArgs const& args);
  WorkspaceQuatInverse(ImTextureID headerBackground, std::string headerLabel = "inverse quat", std::string nodeLabel = "inverse quat");

  void drawDataSetValues(util::NodeBuilder& builder);

};

