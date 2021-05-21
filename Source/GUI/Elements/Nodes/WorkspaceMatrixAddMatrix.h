#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixAddMatrixArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default MatrixAddMatrix header";
    std::string nodeLabel = "mat + mat";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MatrixAddMatrix>();
};

class WorkspaceMatrixAddMatrix : public WorkspaceMatrix4x4
{
public:
    WorkspaceMatrixAddMatrix(ImTextureID headerBackground, WorkspaceMatrixAddMatrixArgs const& args);
    WorkspaceMatrixAddMatrix(ImTextureID headerBackground, std::string headerLabel = "mat + mat", std::string nodeLabel = "mat + mat");

    void drawDataSetValues(util::NodeBuilder& builder);
};