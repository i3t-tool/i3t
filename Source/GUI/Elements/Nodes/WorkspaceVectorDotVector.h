#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceVectorDotVectorArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default WorkspaceVectorDotVector header";
    std::string nodeLabel = "WorkspaceVectorDotVector";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorDotVector>();
};

class WorkspaceVectorDotVector : public WorkspaceFloat
{
public:
    WorkspaceVectorDotVector(ImTextureID headerBackground, WorkspaceVectorDotVectorArgs const& args);
    WorkspaceVectorDotVector(ImTextureID headerBackground, std::string headerLabel = "WorkspaceVectorDotVector", std::string nodeLabel = "WorkspaceVectorDotVector");

    void drawDataSetValues(util::NodeBuilder& builder);
};