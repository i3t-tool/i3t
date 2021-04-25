#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceNormalizeVector3Args
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default FreeVector3 header";
    std::string nodeLabel = "default FreeVector3 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::NormalizeVector3>();
};

class WorkspaceNormalizeVector3 : public WorkspaceVector3
{
public:
    WorkspaceNormalizeVector3(ImTextureID headerBackground, WorkspaceNormalizeVector3Args const& args);
    WorkspaceNormalizeVector3(ImTextureID headerBackground, std::string headerLabel = "NormalizeVector3", std::string nodeLabel = "NormalizeVector3");

    void drawDataSetValues(util::NodeBuilder& builder);

};

