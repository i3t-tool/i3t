#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceVector3MulFloatArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Vector3MulFloat header";
    std::string nodeLabel = "default float * vec3 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3MulFloat>();
};

class WorkspaceVector3MulFloat : public WorkspaceVector3
{
public:
    WorkspaceVector3MulFloat(ImTextureID headerBackground, WorkspaceVector3MulFloatArgs const& args);
    WorkspaceVector3MulFloat(ImTextureID headerBackground, std::string headerLabel = "float * vec3", std::string nodeLabel = "float * vec3");

    void drawDataSetValues(util::NodeBuilder& builder);

};

