#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVectorMulMatrixArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default VectorMulMatrix header";
    std::string nodeLabel = "VectorMulMatrix";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorMulMatrix>();
};

class WorkspaceVectorMulMatrix : public WorkspaceVector4
{
public:
    WorkspaceVectorMulMatrix(ImTextureID headerBackground, WorkspaceVectorMulMatrixArgs const& args);
    WorkspaceVectorMulMatrix(ImTextureID headerBackground, std::string headerLabel = "VectorMulMatrix", std::string nodeLabel = "VectorMulMatrix");

    void drawDataSetValues(util::NodeBuilder& builder);
};