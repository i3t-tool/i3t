#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixAddMatrixArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default MatrixAddMatrix header";
    std::string nodeLabel = "MatrixAddMatrix";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::MatrixAddMatrix>();
};

class WorkspaceMatrixAddMatrix : public WorkspaceMatrix4x4
{
public:
    WorkspaceMatrixAddMatrix(ImTextureID headerBackground, WorkspaceMatrixAddMatrixArgs const& args);
    WorkspaceMatrixAddMatrix(ImTextureID headerBackground, std::string headerLabel = "MatrixAddMatrix", std::string nodeLabel = "MatrixAddMatrix");

    void drawDataSetValues(util::NodeBuilder& builder);
};