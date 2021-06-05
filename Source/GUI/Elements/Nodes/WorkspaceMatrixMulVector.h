#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceMatrixMulVectorArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default MatrixMulVector header";
    std::string nodeLabel = "mat * vec4";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MatrixMulVector>();
};

class WorkspaceMatrixMulVector : public WorkspaceVector4
{
public:
    WorkspaceMatrixMulVector(ImTextureID headerBackground, WorkspaceMatrixMulVectorArgs const& args);
    WorkspaceMatrixMulVector(ImTextureID headerBackground, std::string headerLabel = "mat * vec4", std::string nodeLabel = "mat * vec4");

    void drawDataSetValues(util::NodeBuilder& builder);
};