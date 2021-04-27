#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceVector3FreeArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default FreeVector3 header";
    std::string nodeLabel = "default FreeVector3 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3>();
};

class WorkspaceVector3Free : public WorkspaceVector3
{
public:
    WorkspaceVector3Free(ImTextureID headerBackground, WorkspaceVector3FreeArgs const& args);
    WorkspaceVector3Free(ImTextureID headerBackground, std::string headerLabel = "Vector3", std::string nodeLabel = "Vector3");

    void drawDataSetValues(util::NodeBuilder& builder);

};

