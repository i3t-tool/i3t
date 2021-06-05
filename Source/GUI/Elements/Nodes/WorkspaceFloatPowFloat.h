//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceFloatPowFloatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default float ^ float header";
  std::string nodeLabel = "default float ^ float label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatPowFloat>();
};

class WorkspaceFloatPowFloat : public WorkspaceFloat
{
public:
  WorkspaceFloatPowFloat(ImTextureID headerBackground, WorkspaceFloatPowFloatArgs const& args);
  WorkspaceFloatPowFloat(ImTextureID headerBackground, std::string headerLabel = "float ^ float", std::string nodeLabel = "float ^ float");

  void drawDataSetValues(util::NodeBuilder& builder);
};

