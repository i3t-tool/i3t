//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceFloatMulQuatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Float * Quat header";
  std::string nodeLabel = "default Float * Quat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatMulQuat>();
};

class WorkspaceFloatMulQuat : public WorkspaceQuat
{
public:
  WorkspaceFloatMulQuat(ImTextureID headerBackground, WorkspaceFloatMulQuatArgs const& args);
  WorkspaceFloatMulQuat(ImTextureID headerBackground, std::string headerLabel = "Float * Quat", std::string nodeLabel = "Float * Quat");

  void drawDataSetValues(util::NodeBuilder& builder);

};
