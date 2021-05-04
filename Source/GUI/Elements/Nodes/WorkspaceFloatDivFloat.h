//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceFloatDivFloatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default FloatDivFloat header";
  std::string nodeLabel = "default FloatDivFloat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatDivFloat>();
};

class WorkspaceFloatDivFloat : public WorkspaceFloat
{
public:
  WorkspaceFloatDivFloat(ImTextureID headerBackground, WorkspaceFloatDivFloatArgs const& args);
  WorkspaceFloatDivFloat(ImTextureID headerBackground, std::string headerLabel = "FloatDivFloat", std::string nodeLabel = "FloatDivFloat");

  void drawDataSetValues(util::NodeBuilder& builder);
};

