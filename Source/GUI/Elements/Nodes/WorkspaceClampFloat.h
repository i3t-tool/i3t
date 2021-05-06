//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceClampFloatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default ClampFloat header";
  std::string nodeLabel = "default ClampFloat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::ClampFloat>();
};

class WorkspaceClampFloat : public WorkspaceFloat
{
public:
  WorkspaceClampFloat(ImTextureID headerBackground, WorkspaceClampFloatArgs const& args);
  WorkspaceClampFloat(ImTextureID headerBackground, std::string headerLabel = "ClampFloat", std::string nodeLabel = "ClampFloat");

  void drawDataSetValues(util::NodeBuilder& builder);
};

