//
// Created by Sofie on 15.05.2021.
//
#pragma once
#include "WorkspaceNodeWithCoreData.h"

struct WorkspacePulseArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Pulse header";
  std::string nodeLabel = "Pulse";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Pulse>();
};


class WorkspacePulse : public WorkspaceNodeWithCoreData
{
public:
  WorkspacePulse(ImTextureID headerBackground, WorkspacePulseArgs const& args);
  WorkspacePulse(ImTextureID headerBackground, std::string headerLabel = "Pulse", std::string nodeLabel = "Pulse");


  void drawDataFull(util::NodeBuilder& builder, int index);
  void drawDataSetValues(util::NodeBuilder& builder);
  int maxLenghtOfData();
};
