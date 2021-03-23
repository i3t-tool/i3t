#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixScaleArgs
{
    WorkspaceViewScale viewScale = WorkspaceViewScale::Full;
    std::string headerLabel = "default MatrixScale header";
    std::string nodeLabel = "Scale";
    Ptr<Core::NodeBase> nodebase = Builder::createTransform<Core::Scale>();
};

class WorkspaceMatrixScale : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixScale(ImTextureID headerBackground, WorkspaceMatrixScaleArgs const& args);
    WorkspaceMatrixScale(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase = Builder::createTransform<Core::Scale>(), std::string headerLabel = "Scale", std::string nodeLabel = "Scale");

	void drawData(util::NodeBuilder& builder);
};
