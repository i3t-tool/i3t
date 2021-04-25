#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceVector3DotVector3Args
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Vector3DotVector3 header";
    std::string nodeLabel = "default Vector3DotVector3 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3DotVector3>();
};

class WorkspaceVector3DotVector3 : public WorkspaceFloat
{
public:
    WorkspaceVector3DotVector3(ImTextureID headerBackground, WorkspaceVector3DotVector3Args const& args);
    WorkspaceVector3DotVector3(ImTextureID headerBackground, std::string headerLabel = "Vector3DotVector3", std::string nodeLabel = "Vector3DotVector3");

    void drawDataSetValues(util::NodeBuilder& builder);

};

