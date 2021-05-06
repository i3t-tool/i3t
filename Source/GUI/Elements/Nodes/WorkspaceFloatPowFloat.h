//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceFloatPowFloatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default FloatPowFloat header";
  std::string nodeLabel = "default FloatPowFloat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatPowFloat>();
};

class WorkspaceFloatPowFloat : public WorkspaceFloat
{
public:
  WorkspaceFloatPowFloat(ImTextureID headerBackground, WorkspaceFloatPowFloatArgs const& args);
  WorkspaceFloatPowFloat(ImTextureID headerBackground, std::string headerLabel = "FloatPowFloat", std::string nodeLabel = "FloatPowFloat");

  void drawDataSetValues(util::NodeBuilder& builder);
};

