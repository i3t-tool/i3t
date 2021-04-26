#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceVector3SubVector3Args
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Vector3SubVector3 header";
    std::string nodeLabel = "default Vector3SubVector3 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3SubVector3>();
};

class WorkspaceVector3SubVector3 : public WorkspaceVector3
{
public:
    WorkspaceVector3SubVector3(ImTextureID headerBackground, WorkspaceVector3SubVector3Args const& args);
    WorkspaceVector3SubVector3(ImTextureID headerBackground, std::string headerLabel = "Vector3SubVector3", std::string nodeLabel = "Vector3SubVector3");

    void drawDataSetValues(util::NodeBuilder& builder);

};

