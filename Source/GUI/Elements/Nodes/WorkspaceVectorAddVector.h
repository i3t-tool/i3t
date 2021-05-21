#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVectorAddVectorArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default vec4 + vec4 header";
    std::string nodeLabel = "default vec4 + vec4 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorAddVector>();
};

class WorkspaceVectorAddVector : public WorkspaceVector4
{
public:
    WorkspaceVectorAddVector(ImTextureID headerBackground, WorkspaceVectorAddVectorArgs const& args);
    WorkspaceVectorAddVector(ImTextureID headerBackground, std::string headerLabel = "vec4 + vec4", std::string nodeLabel = "vec4 + vec4");

    void drawDataSetValues(util::NodeBuilder& builder);

};

