//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceMatrixToVectorsArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default mat -> vecs4 header";
  std::string nodeLabel = "default mat -> vecs4 label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MatrixToVectors>();
};

class WorkspaceMatrixToVectors : public WorkspaceVector4
{
public:
  WorkspaceMatrixToVectors(ImTextureID headerBackground, WorkspaceMatrixToVectorsArgs const& args);
  WorkspaceMatrixToVectors(ImTextureID headerBackground, std::string headerLabel = "mat -> vecs4", std::string nodeLabel = "mat -> vecs4");

  void drawDataSetValues(util::NodeBuilder& builder);
};

