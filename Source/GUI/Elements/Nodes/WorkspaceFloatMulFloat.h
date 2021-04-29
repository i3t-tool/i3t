//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceFloatMulFloatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default FloatMulFloat header";
  std::string nodeLabel = "default FloatMulFloat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatMulFloat>();
};

class WorkspaceFloatMulFloat : public WorkspaceFloat
{
public:
  WorkspaceFloatMulFloat(ImTextureID headerBackground, WorkspaceFloatMulFloatArgs const& args);
  WorkspaceFloatMulFloat(ImTextureID headerBackground, std::string headerLabel = "FloatMulFloat", std::string nodeLabel = "FloatMulFloat");

  void drawDataSetValues(util::NodeBuilder& builder);
};