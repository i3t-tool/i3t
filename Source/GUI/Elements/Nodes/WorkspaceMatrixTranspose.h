#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixTransposeArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default MatrixTranspose header";
    std::string nodeLabel = "Matrix Transpose";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Transpose>();
};

class WorkspaceMatrixTranspose : public WorkspaceMatrix4x4
{
public:
    WorkspaceMatrixTranspose(ImTextureID headerBackground, WorkspaceMatrixTransposeArgs const& args);
    WorkspaceMatrixTranspose(ImTextureID headerBackground, std::string headerLabel = "Transpose", std::string nodeLabel = "Transpose");

    void drawDataSetValues(util::NodeBuilder& builder);
};