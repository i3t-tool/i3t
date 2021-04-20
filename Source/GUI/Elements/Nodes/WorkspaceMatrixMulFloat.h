#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixMulFloatArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default MatrixMulFloat header";
    std::string nodeLabel = "MatrixMulFloat";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::MatrixMulFloat>();
};

class WorkspaceMatrixMulFloat : public WorkspaceMatrix4x4
{
public:
    WorkspaceMatrixMulFloat(ImTextureID headerBackground, WorkspaceMatrixMulFloatArgs const& args);
    WorkspaceMatrixMulFloat(ImTextureID headerBackground, std::string headerLabel = "MatrixMulFloat", std::string nodeLabel = "MatrixMulFloat");

    void drawDataSetValues(util::NodeBuilder& builder);
};