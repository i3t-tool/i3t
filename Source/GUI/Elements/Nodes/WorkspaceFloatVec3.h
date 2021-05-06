//
// Created by Sofie on 06.05.2021.
//

#pragma once
#include "WorkspaceNodeWithCoreData.h"

struct WorkspaceFloatVec3Args
{
  WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
  std::string headerLabel = "default FloatVec3 header";
  std::string nodeLabel = "FloatVec3";
  //Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::QuatToAngleAxis>(); //tell MH to make new type?
};


class WorkspaceFloatVec3 : public WorkspaceNodeWithCoreData
{
public:
  WorkspaceFloatVec3(ImTextureID headerBackground, WorkspaceFloatVec3Args const& args);
  WorkspaceFloatVec3(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel);

  virtual void drawDataFull(util::NodeBuilder& builder, int index);
  int maxLenghtOfData();

};

