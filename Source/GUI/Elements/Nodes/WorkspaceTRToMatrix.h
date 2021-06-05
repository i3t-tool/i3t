//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceTRToMatrixArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default TR -> mat header";
  std::string nodeLabel = "TR -> mat";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::TRToMatrix>();
};

class WorkspaceTRToMatrix : public WorkspaceMatrix4x4
{
public:
  WorkspaceTRToMatrix(ImTextureID headerBackground, WorkspaceTRToMatrixArgs const& args);
  WorkspaceTRToMatrix(ImTextureID headerBackground, std::string headerLabel = "TR -> mat", std::string nodeLabel = "TR -> mat");

  void drawDataSetValues(util::NodeBuilder& builder);

};
