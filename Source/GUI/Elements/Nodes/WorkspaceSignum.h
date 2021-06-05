//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceSignumArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Signum header";
  std::string nodeLabel = "default Signum label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Signum>();
};

class WorkspaceSignum : public WorkspaceFloat
{
public:
  WorkspaceSignum(ImTextureID headerBackground, WorkspaceSignumArgs const& args);
  WorkspaceSignum(ImTextureID headerBackground, std::string headerLabel = "Signum", std::string nodeLabel = "Signum");

  void drawDataSetValues(util::NodeBuilder& builder);
};

