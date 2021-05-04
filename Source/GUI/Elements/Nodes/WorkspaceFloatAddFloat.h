//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceFloatAddFloatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default FloatAddFloat header";
  std::string nodeLabel = "default FloatAddFloat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatAddFloat>();
};

class WorkspaceFloatAddFloat : public WorkspaceFloat
{
public:
  WorkspaceFloatAddFloat(ImTextureID headerBackground, WorkspaceFloatAddFloatArgs const& args);
  WorkspaceFloatAddFloat(ImTextureID headerBackground, std::string headerLabel = "FloatAddFloat", std::string nodeLabel = "FloatAddFloat");

  void drawDataSetValues(util::NodeBuilder& builder);
};

