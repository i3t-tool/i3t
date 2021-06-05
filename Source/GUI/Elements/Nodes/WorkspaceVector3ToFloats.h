//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceVector3ToFloatsArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default vec3 -> floats header";
  std::string nodeLabel = "default vec3 -> floats label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3ToFloats>();
};

class WorkspaceVector3ToFloats : public WorkspaceFloat
{
public:
  WorkspaceVector3ToFloats(ImTextureID headerBackground, WorkspaceVector3ToFloatsArgs const& args);
  WorkspaceVector3ToFloats(ImTextureID headerBackground, std::string headerLabel = "vec3 -> floats", std::string nodeLabel = "vec3 -> floats");

  void drawDataSetValues(util::NodeBuilder& builder);
};

