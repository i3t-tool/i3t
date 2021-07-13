#pragma once
#include "WorkspaceMatrix4x4.h"

#include <../Source/DIWNE/diwne.h>
#include <../Source/DIWNE/Node.h>


struct WorkspaceMatrixTranslationArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Translation header";
    std::string nodeLabel = "Translation";
};

class WorkspaceMatrixTranslation : public WorkspaceMatrix4x4, public DIWNE::Node
{
public:
	WorkspaceMatrixTranslation(ImTextureID headerBackground, WorkspaceMatrixTranslationArgs const& args, DIWNE::Diwne * diwne);
    WorkspaceMatrixTranslation(ImTextureID headerBackground, DIWNE::Diwne * diwne, std::string headerLabel = "Translation", std::string nodeLabel = "Translation");

	void drawDataSetValues(util::NodeBuilder& builder);
	bool drawTop();
};
