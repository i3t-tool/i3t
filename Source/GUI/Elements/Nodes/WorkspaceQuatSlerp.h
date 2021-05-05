//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceQuatSlerpArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default slerp header";
  std::string nodeLabel = "default slerp label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatSlerp>();
};

class WorkspaceQuatSlerp : public WorkspaceQuat
{
public:
  WorkspaceQuatSlerp(ImTextureID headerBackground, WorkspaceQuatSlerpArgs const& args);
  WorkspaceQuatSlerp(ImTextureID headerBackground, std::string headerLabel = "slerp", std::string nodeLabel = "slerp");

  void drawDataSetValues(util::NodeBuilder& builder);

};

