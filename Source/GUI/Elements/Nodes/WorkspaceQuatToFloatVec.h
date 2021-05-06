//
// Created by Sofie on 05.05.2021.
//

#pragma once
#include "WorkspaceFloatVec3.h"

struct WorkspaceQuatToFloatVecArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default quat -> float, vec3 header";
  std::string nodeLabel = "quat -> float, vec3";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatToFloatVec>();
};


class WorkspaceQuatToFloatVec : public WorkspaceFloatVec3
{
public:
  WorkspaceQuatToFloatVec(ImTextureID headerBackground, WorkspaceQuatToFloatVecArgs const& args);
  WorkspaceQuatToFloatVec(ImTextureID headerBackground, std::string headerLabel = "quat -> float, vec3", std::string nodeLabel = "quat -> float, vec3");

  void drawDataSetValues(util::NodeBuilder& builder);

};

