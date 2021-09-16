#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceVector3FreeArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Vector3 header";
    std::string nodeLabel = "Vector3";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3ToVector3>();
};

class WorkspaceVector3Free : public WorkspaceVector3
{
public:
    WorkspaceVector3Free(ImTextureID headerBackground, WorkspaceVector3FreeArgs const& args);
    WorkspaceVector3Free(ImTextureID headerBackground, std::string headerLabel = "Vector 3", std::string nodeLabel = "Vector 3");

    void drawDataSetValues(util::NodeBuilder& builder);

};

