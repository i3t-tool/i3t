#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixFreeArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Matrix header";
    std::string nodeLabel = "Matrix";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::MatrixToMatrix>();
};

class WorkspaceMatrixFree : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixFree(ImTextureID headerBackground, WorkspaceMatrixFreeArgs const& args);
	WorkspaceMatrixFree(ImTextureID headerBackground, std::string headerLabel = "Free", std::string nodeLabel = "Free");

    void drawDataSetValues(util::NodeBuilder& builder);

};
