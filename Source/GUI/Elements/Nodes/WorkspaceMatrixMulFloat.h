#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixMulFloatArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default MatrixMulFloat header";
    std::string nodeLabel = "float * mat";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MatrixMulFloat>();
};

class WorkspaceMatrixMulFloat : public WorkspaceMatrix4x4
{
public:
    WorkspaceMatrixMulFloat(ImTextureID headerBackground, WorkspaceMatrixMulFloatArgs const& args);
    WorkspaceMatrixMulFloat(ImTextureID headerBackground, std::string headerLabel = "float * mat", std::string nodeLabel = "float * mat");

    void drawDataSetValues(util::NodeBuilder& builder);
};