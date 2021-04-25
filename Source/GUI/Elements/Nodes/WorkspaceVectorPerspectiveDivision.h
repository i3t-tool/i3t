#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVectorPerspectiveDivisionArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default VectorPerspectiveDivision header";
    std::string nodeLabel = "default VectorPerspectiveDivision label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorPerspectiveDivision>();
};

class WorkspaceVectorPerspectiveDivision : public WorkspaceVector4
{
public:
    WorkspaceVectorPerspectiveDivision(ImTextureID headerBackground, WorkspaceVectorPerspectiveDivisionArgs const& args);
    WorkspaceVectorPerspectiveDivision(ImTextureID headerBackground, std::string headerLabel = "VectorPerspectiveDivision", std::string nodeLabel = "VectorPerspectiveDivision");

    void drawDataSetValues(util::NodeBuilder& builder);

};