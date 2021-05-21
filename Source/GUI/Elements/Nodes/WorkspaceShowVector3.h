#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceShowVector3Args
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default ShowVector3 header";
    std::string nodeLabel = "Show Vector3";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::ShowVector3>();
};

class WorkspaceShowVector3 : public WorkspaceMatrix4x4
{
public:
    WorkspaceShowVector3(ImTextureID headerBackground, WorkspaceShowVector3Args const& args);
    WorkspaceShowVector3(ImTextureID headerBackground, std::string headerLabel = "Show Vector3", std::string nodeLabel = "Show Vector3");

    void drawDataSetValues(util::NodeBuilder& builder);

};

