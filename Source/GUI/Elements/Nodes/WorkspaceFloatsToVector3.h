//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceFloatsToVector3Args
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default FloatsToVector3 header";
  std::string nodeLabel = "default FloatsToVector3 label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatsToVector3>();
};

class WorkspaceFloatsToVector3 : public WorkspaceVector3
{
public:
  WorkspaceFloatsToVector3(ImTextureID headerBackground, WorkspaceFloatsToVector3Args const& args);
  WorkspaceFloatsToVector3(ImTextureID headerBackground, std::string headerLabel = "FloatsToVector3", std::string nodeLabel = "FloatsToVector3");

  void drawDataSetValues(util::NodeBuilder& builder);

};

