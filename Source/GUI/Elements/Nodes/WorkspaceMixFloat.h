//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceMixFloatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default MixFloat header";
  std::string nodeLabel = "default MixFloat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MixFloat>();
};

class WorkspaceMixFloat : public WorkspaceFloat
{
public:
  WorkspaceMixFloat(ImTextureID headerBackground, WorkspaceMixFloatArgs const& args);
  WorkspaceMixFloat(ImTextureID headerBackground, std::string headerLabel = "MixFloat", std::string nodeLabel = "MixFloat");

  void drawDataSetValues(util::NodeBuilder& builder);
};

