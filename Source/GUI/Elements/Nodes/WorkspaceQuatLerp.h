//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceQuatLerpArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default lerp header";
  std::string nodeLabel = "default lerp label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatLerp>();
};

class WorkspaceQuatLerp : public WorkspaceQuat
{
public:
  WorkspaceQuatLerp(ImTextureID headerBackground, WorkspaceQuatLerpArgs const& args);
  WorkspaceQuatLerp(ImTextureID headerBackground, std::string headerLabel = "lerp", std::string nodeLabel = "lerp");

  void drawDataSetValues(util::NodeBuilder& builder);

};

