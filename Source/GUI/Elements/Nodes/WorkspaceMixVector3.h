#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceMixVector3Args
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default MixVector3 header";
    std::string nodeLabel = "default MixVector3 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MixVector3>();
};

class WorkspaceMixVector3 : public WorkspaceVector3
{
public:
    WorkspaceMixVector3(ImTextureID headerBackground, WorkspaceMixVector3Args const& args);
    WorkspaceMixVector3(ImTextureID headerBackground, std::string headerLabel = "MixVector3", std::string nodeLabel = "MixVector3");

    void drawDataSetValues(util::NodeBuilder& builder);

};

