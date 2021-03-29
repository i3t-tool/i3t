#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixTranslationArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Translation header";
    std::string nodeLabel = "Translation";
    Ptr<Core::NodeBase> nodebase = Builder::createTransform<Core::Translation>();
};

class WorkspaceMatrixTranslation : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixTranslation(ImTextureID headerBackground, WorkspaceMatrixTranslationArgs const& args);
    WorkspaceMatrixTranslation(ImTextureID headerBackground, std::string headerLabel = "Translation", std::string nodeLabel = "Translation");

	void drawDataSetValues(util::NodeBuilder& builder);

	ValueSetResult setValueX(float val);
    ValueSetResult setValueY(float val);
    ValueSetResult setValueZ(float val);
    float getValueX();
    float getValueY();
    float getValueZ();

};
