#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceVector3CrossVector3Args
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Vector3CrossVector3 header";
    std::string nodeLabel = "default vec3 x vec3 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3CrossVector3>();
};

class WorkspaceVector3CrossVector3 : public WorkspaceVector3
{
public:
    WorkspaceVector3CrossVector3(ImTextureID headerBackground, WorkspaceVector3CrossVector3Args const& args);
    WorkspaceVector3CrossVector3(ImTextureID headerBackground, std::string headerLabel = "vec3 x vec3", std::string nodeLabel = "vec3 x vec3");

    void drawDataSetValues(util::NodeBuilder& builder);

};

