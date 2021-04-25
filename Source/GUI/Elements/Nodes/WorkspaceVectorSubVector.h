#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVectorSubVectorArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default VectorSubVector header";
    std::string nodeLabel = "default VectorSubVector label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorSubVector>();
};

class WorkspaceVectorSubVector : public WorkspaceVector4
{
public:
    WorkspaceVectorSubVector(ImTextureID headerBackground, WorkspaceVectorSubVectorArgs const& args);
    WorkspaceVectorSubVector(ImTextureID headerBackground, std::string headerLabel = "VectorSubVector", std::string nodeLabel = "VectorSubVector");

    void drawDataSetValues(util::NodeBuilder& builder);

};

