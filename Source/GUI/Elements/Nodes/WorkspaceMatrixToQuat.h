//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceMatrixToQuatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default matrix -> quat header";
  std::string nodeLabel = "default matrix -> quat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MatrixToQuat>();
};

class WorkspaceMatrixToQuat : public WorkspaceQuat
{
public:
  WorkspaceMatrixToQuat(ImTextureID headerBackground, WorkspaceMatrixToQuatArgs const& args);
  WorkspaceMatrixToQuat(ImTextureID headerBackground, std::string headerLabel = "matrix -> quat", std::string nodeLabel = "matrix -> quat");

  void drawDataSetValues(util::NodeBuilder& builder);

};
