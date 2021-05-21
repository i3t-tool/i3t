#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMakeLookAtArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default LookAt header";
    std::string nodeLabel = "Look At";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MakeLookAt>();
};

class WorkspaceMakeLookAt : public WorkspaceMatrix4x4
{
public:
    WorkspaceMakeLookAt(ImTextureID headerBackground, WorkspaceMakeLookAtArgs const& args);
    WorkspaceMakeLookAt(ImTextureID headerBackground, std::string headerLabel = "Look At", std::string nodeLabel = "Look At");

    void drawDataSetValues(util::NodeBuilder& builder);

};
