//
// Created by Sofie on 07.05.2021.
//

#pragma once
#include "WorkspaceNodeWithCoreData.h"

struct WorkspaceCycleArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Cycle header";
  std::string nodeLabel = "default Cycle label";
  Ptr<Core::NodeBase> nodebase = Core::GraphManager::createCycle();
};

class WorkspaceCycle : public WorkspaceNodeWithCoreData
{
public:
  WorkspaceCycle(ImTextureID headerBackground, WorkspaceCycleArgs const& args);
  WorkspaceCycle(ImTextureID headerBackground, Ptr<Core::Cycle> nodebase = nullptr, std::string headerLabel = "default Cycle header", std::string nodeLabel = "Cycle");

	bool isCycle();

  void drawDataSetValues(util::NodeBuilder& builder);
  void drawDataFull(util::NodeBuilder& builder, int index);
  void drawInputPin(util::NodeBuilder& builder, Ptr<WorkspaceCorePinProperties> const & pinProp, Core::Pin* newLinkPin);

	int maxLenghtOfData();
};

