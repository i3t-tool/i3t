#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceVectorDotVectorArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default vec4 . vec4 header";
    std::string nodeLabel = "vec4 . vec4";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorDotVector>();
};

class WorkspaceVectorDotVector : public WorkspaceFloat
{
public:
    WorkspaceVectorDotVector(ImTextureID headerBackground, WorkspaceVectorDotVectorArgs const& args);
    WorkspaceVectorDotVector(ImTextureID headerBackground, std::string headerLabel = "vec4 . vec4", std::string nodeLabel = "vec4 . vec4");

    void drawDataSetValues(util::NodeBuilder& builder);
};