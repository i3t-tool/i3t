//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceNodeWithCoreData.h"

struct WorkspaceQuatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Quat header";
  std::string nodeLabel = "Quat";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatToQuat>();
};

class WorkspaceQuat : public WorkspaceNodeWithCoreData
{
public:
  WorkspaceQuat(ImTextureID headerBackground, WorkspaceQuatArgs const& args);
  WorkspaceQuat(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel);

  virtual void drawDataFull(util::NodeBuilder& builder, int index);
  int maxLenghtOfData();
};