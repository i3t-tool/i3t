#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceMixVector3Args
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Mix Vector3 header";
    std::string nodeLabel = "default Mix Vector3 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MixVector3>();
};

class WorkspaceMixVector3 : public WorkspaceVector3
{
public:
    WorkspaceMixVector3(ImTextureID headerBackground, WorkspaceMixVector3Args const& args);
    WorkspaceMixVector3(ImTextureID headerBackground, std::string headerLabel = "Mix Vector3", std::string nodeLabel = "Mix Vector3");

    void drawDataSetValues(util::NodeBuilder& builder);

};

