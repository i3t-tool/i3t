#pragma once
#include "WorkspaceVector3.h"

struct WorkspaceVector3CrossVector3Args
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Vector3CrossVector3 header";
    std::string nodeLabel = "default Vector3CrossVector3 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3CrossVector3>();
};

class WorkspaceVector3CrossVector3 : public WorkspaceVector3
{
public:
    WorkspaceVector3CrossVector3(ImTextureID headerBackground, WorkspaceVector3CrossVector3Args const& args);
    WorkspaceVector3CrossVector3(ImTextureID headerBackground, std::string headerLabel = "Vector3CrossVector3", std::string nodeLabel = "Vector3CrossVector3");

    void drawDataSetValues(util::NodeBuilder& builder);

};

