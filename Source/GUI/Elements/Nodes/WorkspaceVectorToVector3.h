//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceVectorToVector3Args
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default VectorToVector3 header";
  std::string nodeLabel = "default VectorToVector3 label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorToVector3>();
};

class WorkspaceVectorToVector3 : public WorkspaceVector3
{
public:
  WorkspaceVectorToVector3(ImTextureID headerBackground, WorkspaceVectorToVector3Args const& args);
  WorkspaceVectorToVector3(ImTextureID headerBackground, std::string headerLabel = "VectorToVector3", std::string nodeLabel = "VectorToVector3");

  void drawDataSetValues(util::NodeBuilder& builder);

};

