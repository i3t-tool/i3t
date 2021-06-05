#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceVector3AddVector3Args
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Vector3AddVector3 header";
    std::string nodeLabel = "default vec3 + vec3 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3AddVector3>();
};

class WorkspaceVector3AddVector3 : public WorkspaceVector3
{
public:
    WorkspaceVector3AddVector3(ImTextureID headerBackground, WorkspaceVector3AddVector3Args const& args);
    WorkspaceVector3AddVector3(ImTextureID headerBackground, std::string headerLabel = "vec3 + vec3", std::string nodeLabel = "vec3 + vec3");

    void drawDataSetValues(util::NodeBuilder& builder);

};

#pragma once
