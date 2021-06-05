#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVectorPerspectiveDivisionArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default perspective division header";
    std::string nodeLabel = "default perspective division label";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorPerspectiveDivision>();
};

class WorkspaceVectorPerspectiveDivision : public WorkspaceVector4
{
public:
    WorkspaceVectorPerspectiveDivision(ImTextureID headerBackground, WorkspaceVectorPerspectiveDivisionArgs const& args);
    WorkspaceVectorPerspectiveDivision(ImTextureID headerBackground, std::string headerLabel = "Perspective division", std::string nodeLabel = "Perspective division");

    void drawDataSetValues(util::NodeBuilder& builder);

};