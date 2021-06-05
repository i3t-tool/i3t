//
// Created by Sofie on 29.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceLookAtArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Look At header";
  std::string nodeLabel = "Look At";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createTransform<Core::LookAt>();
};

class WorkspaceLookAt : public WorkspaceMatrix4x4
{
public:
  WorkspaceLookAt(ImTextureID headerBackground, WorkspaceLookAtArgs const& args);
  WorkspaceLookAt(ImTextureID headerBackground, std::string headerLabel = "Look At", std::string nodeLabel = "Look At");

  void drawDataSetValues(util::NodeBuilder& builder);
};
