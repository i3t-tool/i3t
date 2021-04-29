//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceFloatsToVectorArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default FloatsToVector header";
  std::string nodeLabel = "default FloatsToVector label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatsToVector>();
};

class WorkspaceFloatsToVector : public WorkspaceVector4
{
public:
  WorkspaceFloatsToVector(ImTextureID headerBackground, WorkspaceFloatsToVectorArgs const& args);
  WorkspaceFloatsToVector(ImTextureID headerBackground, std::string headerLabel = "FloatsToVector", std::string nodeLabel = "FloatsToVector");

  void drawDataSetValues(util::NodeBuilder& builder);

};


