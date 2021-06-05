//
// Created by Sofie on 29.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceTransformationFreeArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Free header";
  std::string nodeLabel = "Free";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createTransform<Core::Free>();
};

class WorkspaceTransformationFree : public WorkspaceMatrix4x4
{
public:
  WorkspaceTransformationFree(ImTextureID headerBackground, WorkspaceTransformationFreeArgs const& args);
  WorkspaceTransformationFree(ImTextureID headerBackground, std::string headerLabel = "Free", std::string nodeLabel = "Free");

  void drawDataSetValues(util::NodeBuilder& builder);
};
