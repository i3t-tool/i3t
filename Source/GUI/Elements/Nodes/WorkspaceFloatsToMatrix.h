//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceFloatsToMatrixArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default FloatsToMatrix header";
  std::string nodeLabel = "FloatsToMatrix";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatsToMatrix>();
};

class WorkspaceFloatsToMatrix : public WorkspaceMatrix4x4
{
public:
  WorkspaceFloatsToMatrix(ImTextureID headerBackground, WorkspaceFloatsToMatrixArgs const& args);
  WorkspaceFloatsToMatrix(ImTextureID headerBackground, std::string headerLabel = "FloatsToMatrix", std::string nodeLabel = "FloatsToMatrix");

  void drawDataSetValues(util::NodeBuilder& builder);

};
