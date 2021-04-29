//
// Created by Sofie on 29.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceEulerYArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Euler Y header";
  std::string nodeLabel = "Euler Y";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createTransform<Core::EulerRotY>();
};

class WorkspaceEulerY : public WorkspaceMatrix4x4
{
public:
  WorkspaceEulerY(ImTextureID headerBackground, WorkspaceEulerYArgs const& args);
  WorkspaceEulerY(ImTextureID headerBackground, std::string headerLabel = "Euler Y", std::string nodeLabel = "Euler Y");

  void drawDataSetValues(util::NodeBuilder& builder);
};
