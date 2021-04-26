#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVectorMulFloatArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default VectorMulFloat header";
    std::string nodeLabel = "default VectorMulFloat label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorMulFloat>();
};

class WorkspaceVectorMulFloat : public WorkspaceVector4
{
public:
    WorkspaceVectorMulFloat(ImTextureID headerBackground, WorkspaceVectorMulFloatArgs const& args);
    WorkspaceVectorMulFloat(ImTextureID headerBackground, std::string headerLabel = "VectorMulFloat", std::string nodeLabel = "VectorMulFloat");

    void drawDataSetValues(util::NodeBuilder& builder);

};

