//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVector3ToVectorArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default vec3 -> vec4 header";
  std::string nodeLabel = "default vec3 -> vec4 label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3ToVector>();
};

class WorkspaceVector3ToVector : public WorkspaceVector4
{
public:
  WorkspaceVector3ToVector(ImTextureID headerBackground, WorkspaceVector3ToVectorArgs const& args);
  WorkspaceVector3ToVector(ImTextureID headerBackground, std::string headerLabel = "vec3 -> vec4", std::string nodeLabel = "vec3 -> vec4");

  void drawDataSetValues(util::NodeBuilder& builder);

};


