//
// Created by Sofie on 29.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceEulerZArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Euler Z header";
  std::string nodeLabel = "Euler Z";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createTransform<Core::EulerRotZ>();
};

class WorkspaceEulerZ : public WorkspaceMatrix4x4
{
public:
  WorkspaceEulerZ(ImTextureID headerBackground, WorkspaceEulerZArgs const& args);
  WorkspaceEulerZ(ImTextureID headerBackground, std::string headerLabel = "Euler Z", std::string nodeLabel = "Euler Z");

  void drawDataSetValues(util::NodeBuilder& builder);
};
