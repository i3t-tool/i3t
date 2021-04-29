//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVector3ToVectorArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Vector3ToVector header";
  std::string nodeLabel = "default Vector3ToVector label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3ToVector>();
};

class WorkspaceVector3ToVector : public WorkspaceVector4
{
public:
  WorkspaceVector3ToVector(ImTextureID headerBackground, WorkspaceVector3ToVectorArgs const& args);
  WorkspaceVector3ToVector(ImTextureID headerBackground, std::string headerLabel = "Vector3ToVector", std::string nodeLabel = "Vector3ToVector");

  void drawDataSetValues(util::NodeBuilder& builder);

};


