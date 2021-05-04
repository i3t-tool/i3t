#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceQuatToEulerArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default QuatToEuler header";
  std::string nodeLabel = "default QuatToEuler label";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatToEuler>();
};

class WorkspaceQuatToEuler : public WorkspaceFloat
{
public:
  WorkspaceQuatToEuler(ImTextureID headerBackground, WorkspaceQuatToEulerArgs const& args);
  WorkspaceQuatToEuler(ImTextureID headerBackground, std::string headerLabel = "QuatToEuler", std::string nodeLabel = "QuatToEuler");

  void drawDataSetValues(util::NodeBuilder& builder);
};

