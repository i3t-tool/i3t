//
// Created by Sofie on 04.05.2021.
//

#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceQuatVecConjQuatArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default qvq* header";
  std::string nodeLabel = "default qvq* label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatVecConjQuat>();
};

class WorkspaceQuatVecConjQuat : public WorkspaceVector3
{
public:
  WorkspaceQuatVecConjQuat(ImTextureID headerBackground, WorkspaceQuatVecConjQuatArgs const& args);
  WorkspaceQuatVecConjQuat(ImTextureID headerBackground, std::string headerLabel = "qvq*", std::string nodeLabel = "qvq*");

  void drawDataSetValues(util::NodeBuilder& builder);

};

