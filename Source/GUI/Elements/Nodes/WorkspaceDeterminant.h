#pragma once
#include "WorkspaceFloat.h"

struct WorkspaceDeterminantArgs
{
    WorkspaceViewScale viewScale = WorkspaceViewScale::Full;
    std::string headerLabel = "default Determinant header";
    std::string nodeLabel = "Determinant";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::Determinant>();
};


class WorkspaceDeterminant : public WorkspaceFloat
{
public:
    WorkspaceDeterminant(ImTextureID headerBackground, WorkspaceDeterminantArgs const& args);
    WorkspaceDeterminant(ImTextureID headerBackground, std::string headerLabel = "Determinant", std::string nodeLabel = "Determinant");

    void drawData(util::NodeBuilder& builder);
};

