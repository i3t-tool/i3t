#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceQuatToEulerArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default quat -> euler header";
  std::string nodeLabel = "default quat -> euler label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatToEuler>();
};

class WorkspaceQuatToEuler : public WorkspaceFloat
{
public:
  WorkspaceQuatToEuler(ImTextureID headerBackground, WorkspaceQuatToEulerArgs const& args);
  WorkspaceQuatToEuler(ImTextureID headerBackground, std::string headerLabel = "quat -> euler", std::string nodeLabel = "quat -> euler");

  void drawDataSetValues(util::NodeBuilder& builder);
};

