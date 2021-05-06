//
// Created by Sofie on 05.05.2021.
//
#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceQuatToFloatsArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default quat -> floats header";
  std::string nodeLabel = "default quat -> floats label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatToFloats>();
};

class WorkspaceQuatToFloats : public WorkspaceFloat
{
public:
  WorkspaceQuatToFloats(ImTextureID headerBackground, WorkspaceQuatToFloatsArgs const& args);
  WorkspaceQuatToFloats(ImTextureID headerBackground, std::string headerLabel = "quat -> floats", std::string nodeLabel = "quat -> floats");

  void drawDataSetValues(util::NodeBuilder& builder);
};

