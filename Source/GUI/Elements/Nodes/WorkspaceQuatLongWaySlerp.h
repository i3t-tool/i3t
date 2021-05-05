//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceQuatLongWaySlerpArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default long way slerp header";
  std::string nodeLabel = "default long way slerp label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatLongWaySlerp>();
};

class WorkspaceQuatLongWaySlerp : public WorkspaceQuat
{
public:
  WorkspaceQuatLongWaySlerp(ImTextureID headerBackground, WorkspaceQuatLongWaySlerpArgs const& args);
  WorkspaceQuatLongWaySlerp(ImTextureID headerBackground, std::string headerLabel = "long way slerp", std::string nodeLabel = "long way slerp");

  void drawDataSetValues(util::NodeBuilder& builder);

};

