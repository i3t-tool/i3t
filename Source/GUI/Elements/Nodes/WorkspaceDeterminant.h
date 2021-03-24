#pragma once
#include "WorkspaceNodeWithCoreData.h"

struct WorkspaceDeterminantArgs
{
    WorkspaceViewScale viewScale = WorkspaceViewScale::Full;
    std::string headerLabel = "default Determinant header";
    std::string nodeLabel = "Determinant";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::Determinant>();
};


class WorkspaceDeterminant : public WorkspaceNodeWithCoreData
{
public:
    WorkspaceDeterminant(ImTextureID headerBackground, WorkspaceDeterminantArgs const& args);

    virtual void drawDataFull(util::NodeBuilder& builder);
};

