//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceQuatToMatrixArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default quat -> mat header";
  std::string nodeLabel = "quat -> mat";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatToMatrix>();
};

class WorkspaceQuatToMatrix : public WorkspaceMatrix4x4
{
public:
  WorkspaceQuatToMatrix(ImTextureID headerBackground, WorkspaceQuatToMatrixArgs const& args);
  WorkspaceQuatToMatrix(ImTextureID headerBackground, std::string headerLabel = "quat -> mat", std::string nodeLabel = "quat -> mat");

  void drawDataSetValues(util::NodeBuilder& builder);
};

