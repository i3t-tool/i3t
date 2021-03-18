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

	void drawData(util::NodeBuilder& builder);

};


