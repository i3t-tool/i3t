//
// Created by Sofie on 30.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspacePerspectiveArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Perspective header";
  std::string nodeLabel = "Perspective";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createTransform<Core::PerspectiveProj>();
};

class WorkspacePerspective : public WorkspaceMatrix4x4
{
public:
  WorkspacePerspective(ImTextureID headerBackground, WorkspacePerspectiveArgs const& args);
  WorkspacePerspective(ImTextureID headerBackground, std::string headerLabel = "Perspective", std::string nodeLabel = "Perspective");

  void drawDataSetValues(util::NodeBuilder& builder);
};

