#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceQuatToEulerArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default FreeFloat header";
  std::string nodeLabel = "default FreeFloat label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatToEuler>();
};

class WorkspaceQuatToEuler : public WorkspaceFloat
{
public:
  WorkspaceQuatToEuler(ImTextureID headerBackground, WorkspaceQuatToEulerArgs const& args);
  WorkspaceQuatToEuler(ImTextureID headerBackground, std::string headerLabel = "Free Float", std::string nodeLabel = "Free Float");

  void drawDataSetValues(util::NodeBuilder& builder);
};

