//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceFloatsToVectorArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default floats -> vec4 header";
  std::string nodeLabel = "default floats -> vec4 label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatsToVector>();
};

class WorkspaceFloatsToVector : public WorkspaceVector4
{
public:
  WorkspaceFloatsToVector(ImTextureID headerBackground, WorkspaceFloatsToVectorArgs const& args);
  WorkspaceFloatsToVector(ImTextureID headerBackground, std::string headerLabel = "floats -> vec4", std::string nodeLabel = "floats -> vec4");

  void drawDataSetValues(util::NodeBuilder& builder);

};


