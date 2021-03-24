#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixTranslationArgs
{
    WorkspaceViewScale viewScale = WorkspaceViewScale::Full;
    std::string headerLabel = "default Translation header";
    std::string nodeLabel = "Translation";
    Ptr<Core::NodeBase> nodebase = Builder::createTransform<Core::Translation>();
};

class WorkspaceMatrixTranslation : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixTranslation(ImTextureID headerBackground, WorkspaceMatrixTranslationArgs const& args);
    WorkspaceMatrixTranslation(ImTextureID headerBackground, std::string headerLabel = "Translation", std::string nodeLabel = "Translation");

	void drawData(util::NodeBuilder& builder);

	void drawDataSetValues(util::NodeBuilder& builder);
	void drawDataLabel(util::NodeBuilder& builder);

};
