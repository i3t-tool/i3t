#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceNormalizeVector3Args
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Normalize vec3 header";
    std::string nodeLabel = "default Normalize vec3 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::NormalizeVector3>();
};

class WorkspaceNormalizeVector3 : public WorkspaceVector3
{
public:
    WorkspaceNormalizeVector3(ImTextureID headerBackground, WorkspaceNormalizeVector3Args const& args);
    WorkspaceNormalizeVector3(ImTextureID headerBackground, std::string headerLabel = "Normalize vec3", std::string nodeLabel = "Normalize vec3");

    void drawDataSetValues(util::NodeBuilder& builder);

};

