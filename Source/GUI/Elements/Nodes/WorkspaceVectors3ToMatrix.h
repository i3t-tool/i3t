//
// Created by Sofie on 28.04.2021.
//
#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceVectors3ToMatrixArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default vecs3 -> mat header";
  std::string nodeLabel = "vecs3 -> mat";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vectors3ToMatrix>();
};

class WorkspaceVectors3ToMatrix : public WorkspaceMatrix4x4
{
public:
  WorkspaceVectors3ToMatrix(ImTextureID headerBackground, WorkspaceVectors3ToMatrixArgs const& args);
  WorkspaceVectors3ToMatrix(ImTextureID headerBackground, std::string headerLabel = "vecs3 -> mat", std::string nodeLabel = "vecs3 -> mat");

  void drawDataSetValues(util::NodeBuilder& builder);

};

