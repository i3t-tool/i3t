//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixToTRArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default mat -> TR header";
  std::string nodeLabel = "default mat -> TR label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MatrixToTR>();
};

class WorkspaceMatrixToTR : public WorkspaceMatrix4x4
{
public:
  WorkspaceMatrixToTR(ImTextureID headerBackground, WorkspaceMatrixToTRArgs const& args);
  WorkspaceMatrixToTR(ImTextureID headerBackground, std::string headerLabel = "mat -> TR", std::string nodeLabel = "mat -> TR");

  void drawDataSetValues(util::NodeBuilder& builder);
};

