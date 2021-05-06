//
// Created by Sofie on 05.05.2021.
//
#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceMatrixToFloatsArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default mat -> floats header";
  std::string nodeLabel = "default mat -> floats label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MatrixToFloats>();
};

class WorkspaceMatrixToFloats : public WorkspaceFloat
{
public:
  WorkspaceMatrixToFloats(ImTextureID headerBackground, WorkspaceMatrixToFloatsArgs const& args);
  WorkspaceMatrixToFloats(ImTextureID headerBackground, std::string headerLabel = "mat -> floats", std::string nodeLabel = "mat -> floats");

  void drawDataSetValues(util::NodeBuilder& builder);
};

