//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceVectorToFloatsArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default vec4 -> floats header";
  std::string nodeLabel = "default vec4 -> floats label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorToFloats>();
};

class WorkspaceVectorToFloats : public WorkspaceFloat
{
public:
  WorkspaceVectorToFloats(ImTextureID headerBackground, WorkspaceVectorToFloatsArgs const& args);
  WorkspaceVectorToFloats(ImTextureID headerBackground, std::string headerLabel = "vec4 -> floats", std::string nodeLabel = "vec4 -> floats");

  void drawDataSetValues(util::NodeBuilder& builder);
};

