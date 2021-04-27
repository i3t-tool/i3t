#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVectorAddVectorArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default VectorAddVector header";
    std::string nodeLabel = "default VectorAddVector label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorAddVector>();
};

class WorkspaceVectorAddVector : public WorkspaceVector4
{
public:
    WorkspaceVectorAddVector(ImTextureID headerBackground, WorkspaceVectorAddVectorArgs const& args);
    WorkspaceVectorAddVector(ImTextureID headerBackground, std::string headerLabel = "VectorAddVector", std::string nodeLabel = "VectorAddVector");

    void drawDataSetValues(util::NodeBuilder& builder);

};

