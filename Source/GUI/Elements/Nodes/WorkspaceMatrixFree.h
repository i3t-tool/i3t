#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixFreeArgs
{
    WorkspaceViewScale viewScale = WorkspaceViewScale::Full;
    std::string headerLabel = "default MatrixScale header";
    std::string nodeLabel = "Scale";
    Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Matrix>();
};

class WorkspaceMatrixFree : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixFree(ImTextureID headerBackground, WorkspaceMatrixFreeArgs const& args);
    WorkspaceMatrixFree(ImTextureID headerBackground, std::string headerLabel = "Free", std::string nodeLabel = "Free");

	void drawData(util::NodeBuilder& builder);
};
