#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceMatrixMulVectorArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default MatrixMulVector header";
    std::string nodeLabel = "MatrixMulVector";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MatrixMulVector>();
};

class WorkspaceMatrixMulVector : public WorkspaceVector4
{
public:
    WorkspaceMatrixMulVector(ImTextureID headerBackground, WorkspaceMatrixMulVectorArgs const& args);
    WorkspaceMatrixMulVector(ImTextureID headerBackground, std::string headerLabel = "MatrixMulVector", std::string nodeLabel = "MatrixMulVector");

    void drawDataSetValues(util::NodeBuilder& builder);
};