#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVectorSubVectorArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default vec4 - vec4 header";
    std::string nodeLabel = "default vec4 - vec4 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorSubVector>();
};

class WorkspaceVectorSubVector : public WorkspaceVector4
{
public:
    WorkspaceVectorSubVector(ImTextureID headerBackground, WorkspaceVectorSubVectorArgs const& args);
    WorkspaceVectorSubVector(ImTextureID headerBackground, std::string headerLabel = "vec4 - vec4", std::string nodeLabel = "vec4 - vec4");

    void drawDataSetValues(util::NodeBuilder& builder);

};

