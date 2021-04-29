//
// Created by Sofie on 29.04.2021.
//

#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceEulerXArgs
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default Euler X header";
  std::string nodeLabel = "Euler X";
  Ptr<Core::NodeBase> nodebase = Core::Builder::createTransform<Core::EulerRotX>();
};

class WorkspaceEulerX : public WorkspaceMatrix4x4
{
public:
  WorkspaceEulerX(ImTextureID headerBackground, WorkspaceEulerXArgs const& args);
  WorkspaceEulerX(ImTextureID headerBackground, std::string headerLabel = "Euler X", std::string nodeLabel = "Euler X");

  void drawDataSetValues(util::NodeBuilder& builder);
  float get11();
  float get12();
  float get21();
  float get22();
};
