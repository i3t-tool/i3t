#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceVectorMulMatrixArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default VectorMulMatrix header";
    std::string nodeLabel = "vec4 * mat";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::VectorMulMatrix>();
};

class WorkspaceVectorMulMatrix : public WorkspaceVector4
{
public:
    WorkspaceVectorMulMatrix(ImTextureID headerBackground, WorkspaceVectorMulMatrixArgs const& args);
    WorkspaceVectorMulMatrix(ImTextureID headerBackground, std::string headerLabel = "vec4 * mat", std::string nodeLabel = "vec4 * mat");

    void drawDataSetValues(util::NodeBuilder& builder);
};