//
// Created by Sofie on 30.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceOrthoArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Ortho header";
  std::string nodeLabel = "Ortho";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createTransform<Core::OrthoProj>();
};

class WorkspaceOrtho : public WorkspaceMatrix4x4
{
public:
  WorkspaceOrtho(ImTextureID headerBackground, WorkspaceOrthoArgs const& args);
  WorkspaceOrtho(ImTextureID headerBackground, std::string headerLabel = "Ortho", std::string nodeLabel = "Ortho");

  void drawDataSetValues(util::NodeBuilder& builder);
};

