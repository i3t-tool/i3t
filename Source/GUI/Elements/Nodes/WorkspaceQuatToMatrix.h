//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceQuatToMatrixArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default QuatToMatrix header";
  std::string nodeLabel = "QuatToMatrix";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatToMatrix>();
};

class WorkspaceQuatToMatrix : public WorkspaceMatrix4x4
{
public:
  WorkspaceQuatToMatrix(ImTextureID headerBackground, WorkspaceQuatToMatrixArgs const& args);
  WorkspaceQuatToMatrix(ImTextureID headerBackground, std::string headerLabel = "QuatToMatrix", std::string nodeLabel = "QuatToMatrix");

  void drawDataSetValues(util::NodeBuilder& builder);
};

