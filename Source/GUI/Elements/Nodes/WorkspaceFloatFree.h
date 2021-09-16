#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceFloatFreeArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Float header";
    std::string nodeLabel = "default Float label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::FloatToFloat>();
};

class WorkspaceFloatFree : public WorkspaceFloat
{
public:
    WorkspaceFloatFree(ImTextureID headerBackground, WorkspaceFloatFreeArgs const& args);
    WorkspaceFloatFree(ImTextureID headerBackground, std::string headerLabel = "Float", std::string nodeLabel = "Float");

    void drawDataSetValues(util::NodeBuilder& builder);
};

