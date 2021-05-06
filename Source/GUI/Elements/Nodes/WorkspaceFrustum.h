//
// Created by Sofie on 30.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceFrustumArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Frustum header";
  std::string nodeLabel = "Frustum";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createTransform<Core::Frustum>();
};

class WorkspaceFrustum : public WorkspaceMatrix4x4
{
public:
  WorkspaceFrustum(ImTextureID headerBackground, WorkspaceFrustumArgs const& args);
  WorkspaceFrustum(ImTextureID headerBackground, std::string headerLabel = "Frustum", std::string nodeLabel = "Frustum");

  void drawDataSetValues(util::NodeBuilder& builder);
};

