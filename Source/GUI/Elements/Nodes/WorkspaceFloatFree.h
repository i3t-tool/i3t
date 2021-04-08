#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceFloatFreeArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default FreeFloat header";
    std::string nodeLabel = "default FreeFloat label";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::Float>();
};

class WorkspaceFloatFree : public WorkspaceFloat
{
public:
    WorkspaceFloatFree(ImTextureID headerBackground, WorkspaceFloatFreeArgs const& args);
    WorkspaceFloatFree(ImTextureID headerBackground, std::string headerLabel = "Free Float", std::string nodeLabel = "Free Float");

    void drawData(util::NodeBuilder& builder);

};

