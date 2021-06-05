#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVectorMulFloatArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default float * vec4 header";
    std::string nodeLabel = "default float * vec4 label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorMulFloat>();
};

class WorkspaceVectorMulFloat : public WorkspaceVector4
{
public:
    WorkspaceVectorMulFloat(ImTextureID headerBackground, WorkspaceVectorMulFloatArgs const& args);
    WorkspaceVectorMulFloat(ImTextureID headerBackground, std::string headerLabel = "float * vec4", std::string nodeLabel = "float * vec4");

    void drawDataSetValues(util::NodeBuilder& builder);

};

