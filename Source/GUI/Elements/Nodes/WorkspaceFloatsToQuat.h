//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceFloatsToQuatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default floats -> quat header";
  std::string nodeLabel = "default floats -> quat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatsToQuat>();
};

class WorkspaceFloatsToQuat : public WorkspaceQuat
{
public:
  WorkspaceFloatsToQuat(ImTextureID headerBackground, WorkspaceQuatFreeArgs const& args);
  WorkspaceFloatsToQuat(ImTextureID headerBackground, std::string headerLabel = "floats -> quat", std::string nodeLabel = "floats -> quat");

  void drawDataSetValues(util::NodeBuilder& builder);

};
