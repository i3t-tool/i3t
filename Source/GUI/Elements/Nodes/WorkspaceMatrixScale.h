#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixScaleArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default MatrixScale header";
    std::string nodeLabel = "Scale";
    Ptr<Core::NodeBase> nodebase = Builder::createTransform<Core::Scale>();
};

class WorkspaceMatrixScale : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixScale(ImTextureID headerBackground, WorkspaceMatrixScaleArgs const& args);
    WorkspaceMatrixScale(ImTextureID headerBackground, std::string headerLabel = "Scale", std::string nodeLabel = "Scale");

	void drawDataSetValues(util::NodeBuilder& builder);

	ValueSetResult setValueX(float val);
    ValueSetResult setValueY(float val);
    ValueSetResult setValueZ(float val);
    float getValueX();
    float getValueY();
    float getValueZ();
};
