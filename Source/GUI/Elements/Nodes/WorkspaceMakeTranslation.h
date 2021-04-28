//
// Created by Sofie on 28.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMakeTranslationArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Translation header";
  std::string nodeLabel = "Translation";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MakeTranslation>();
};

class WorkspaceMakeTranslation : public WorkspaceMatrix4x4
{
public:
  WorkspaceMakeTranslation(ImTextureID headerBackground, WorkspaceMakeTranslationArgs const& args);
  WorkspaceMakeTranslation(ImTextureID headerBackground, std::string headerLabel = "Translation", std::string nodeLabel = "Translation");

  void drawDataSetValues(util::NodeBuilder& builder);

};
