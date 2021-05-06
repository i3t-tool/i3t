//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceASinACosArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default asin & acos(float) header";
  std::string nodeLabel = "default asin & acos(float) label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::ASinACos>();
};

class WorkspaceASinACos : public WorkspaceFloat
{
public:
  WorkspaceASinACos(ImTextureID headerBackground, WorkspaceASinACosArgs const& args);
  WorkspaceASinACos(ImTextureID headerBackground, std::string headerLabel = "asin & acos(float)", std::string nodeLabel = "asin & acos(float)");

  void drawDataSetValues(util::NodeBuilder& builder);
};

