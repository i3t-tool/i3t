//
// Created by Sofie on 05.05.2021.
//
#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceEulerToQuatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default euler -> quat header";
  std::string nodeLabel = "default euler -> quat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::EulerToQuat>();
};

class WorkspaceEulerToQuat : public WorkspaceQuat
{
public:
  WorkspaceEulerToQuat(ImTextureID headerBackground, WorkspaceEulerToQuatArgs const& args);
  WorkspaceEulerToQuat(ImTextureID headerBackground, std::string headerLabel = "euler -> quat", std::string nodeLabel = "euler -> quat");

  void drawDataSetValues(util::NodeBuilder& builder);

};
