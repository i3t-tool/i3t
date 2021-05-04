//
// Created by Sofie on 04.05.2021.
//

#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceQuatLengthArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default length(quat) header";
  std::string nodeLabel = "default length(quat) label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatLength>();
};

class WorkspaceQuatLength : public WorkspaceFloat
{
public:
  WorkspaceQuatLength(ImTextureID headerBackground, WorkspaceQuatLengthArgs const& args);
  WorkspaceQuatLength(ImTextureID headerBackground, std::string headerLabel = "length(quat)", std::string nodeLabel = "length(quat)");

  void drawDataSetValues(util::NodeBuilder& builder);

};

