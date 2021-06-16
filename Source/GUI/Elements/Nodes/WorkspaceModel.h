#pragma once

#include "WorkspaceNodeWithCoreData.h"

struct WorkspaceModelArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Model header";
  std::string nodeLabel = "Model";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Model>();
};


class WorkspaceModel : public WorkspaceNodeWithCoreData
{
public:
  WorkspaceModel(ImTextureID headerBackground, const WorkspaceModelArgs& args);
  explicit WorkspaceModel(ImTextureID headerBackground, std::string headerLabel = "Model", std::string nodeLabel = "Model");

  void drawDataFull(util::NodeBuilder& builder, int index) override;
  void drawDataSetValues(util::NodeBuilder& builder) override;
	int maxLenghtOfData() override;

private:
	int m_currentModelIdx = 0;

	void init();
};
