//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceFloatSinCosArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default sin & cos(float) header";
  std::string nodeLabel = "default sin & cos(float) label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatSinCos>();
};

class WorkspaceFloatSinCos : public WorkspaceFloat
{
public:
  WorkspaceFloatSinCos(ImTextureID headerBackground, WorkspaceFloatSinCosArgs const& args);
  WorkspaceFloatSinCos(ImTextureID headerBackground, std::string headerLabel = "sin & cos(float)", std::string nodeLabel = "sin & cos(float)");

  void drawDataSetValues(util::NodeBuilder& builder);
};

