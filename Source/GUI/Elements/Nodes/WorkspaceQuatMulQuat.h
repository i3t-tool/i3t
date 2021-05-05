//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceQuatMulQuatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Quat * Quat header";
  std::string nodeLabel = "default Quat * Quat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatMulQuat>();
};

class WorkspaceQuatMulQuat : public WorkspaceQuat
{
public:
  WorkspaceQuatMulQuat(ImTextureID headerBackground, WorkspaceQuatMulQuatArgs const& args);
  WorkspaceQuatMulQuat(ImTextureID headerBackground, std::string headerLabel = "Quat * Quat", std::string nodeLabel = "Quat * Quat");

  void drawDataSetValues(util::NodeBuilder& builder);

};

