//
// Created by Sofie on 28.04.2021.
//
#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceVectors3ToMatrixArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Vectors3ToMatrix header";
  std::string nodeLabel = "Vectors3ToMatrix";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vectors3ToMatrix>();
};

class WorkspaceVectors3ToMatrix : public WorkspaceMatrix4x4
{
public:
  WorkspaceVectors3ToMatrix(ImTextureID headerBackground, WorkspaceVectors3ToMatrixArgs const& args);
  WorkspaceVectors3ToMatrix(ImTextureID headerBackground, std::string headerLabel = "Vectors3ToMatrix", std::string nodeLabel = "Vectors3ToMatrix");

  void drawDataSetValues(util::NodeBuilder& builder);

};

