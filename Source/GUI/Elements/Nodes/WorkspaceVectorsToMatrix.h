//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceVectorsToMatrixArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default VectorsToMatrix header";
  std::string nodeLabel = "VectorsToMatrix";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorsToMatrix>();
};

class WorkspaceVectorsToMatrix : public WorkspaceMatrix4x4
{
public:
  WorkspaceVectorsToMatrix(ImTextureID headerBackground, WorkspaceVectorsToMatrixArgs const& args);
  WorkspaceVectorsToMatrix(ImTextureID headerBackground, std::string headerLabel = "VectorsToMatrix", std::string nodeLabel = "VectorsToMatrix");

  void drawDataSetValues(util::NodeBuilder& builder);

};
