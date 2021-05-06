//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceQuat.h"

struct WorkspaceConjQuatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default quat conjugate header";
  std::string nodeLabel = "default quat conjugate label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::ConjQuat>();
};

class WorkspaceConjQuat : public WorkspaceQuat
{
public:
  WorkspaceConjQuat(ImTextureID headerBackground, WorkspaceConjQuatArgs const& args);
  WorkspaceConjQuat(ImTextureID headerBackground, std::string headerLabel = "quat conjugate", std::string nodeLabel = "quat conjugate");

  void drawDataSetValues(util::NodeBuilder& builder);

};

