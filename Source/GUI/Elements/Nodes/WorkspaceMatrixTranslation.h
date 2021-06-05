#pragma once
#include "WorkspaceMatrix4x4.h"

struct WorkspaceMatrixTranslationArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Translation header";
    std::string nodeLabel = "Translation";
};

class WorkspaceMatrixTranslation : public WorkspaceMatrix4x4
{
public:
	WorkspaceMatrixTranslation(ImTextureID headerBackground, WorkspaceMatrixTranslationArgs const& args);
    WorkspaceMatrixTranslation(ImTextureID headerBackground, std::string headerLabel = "Translation", std::string nodeLabel = "Translation");

	void drawDataSetValues(util::NodeBuilder& builder);
};
