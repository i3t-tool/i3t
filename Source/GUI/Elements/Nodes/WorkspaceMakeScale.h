//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMakeScaleArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Scale header";
  std::string nodeLabel = "Scale";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MakeScale>();
};

class WorkspaceMakeScale : public WorkspaceMatrix4x4
{
public:
  WorkspaceMakeScale(ImTextureID headerBackground, WorkspaceMakeScaleArgs const& args);
  WorkspaceMakeScale(ImTextureID headerBackground, std::string headerLabel = "Scale", std::string nodeLabel = "Scale");

  void drawDataSetValues(util::NodeBuilder& builder);

};
