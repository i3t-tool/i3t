#pragma once
#include "WorkspaceNodeWithCoreData.h"

struct WorkspaceSequenceArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default Sequence header";
    std::string nodeLabel = "Sequence";
};

class WorkspaceSequence : public WorkspaceNodeWithCoreData
{
public:
	WorkspaceSequence(ImTextureID headerBackground, WorkspaceSequenceArgs const& args);
    WorkspaceSequence(ImTextureID headerBackground, std::string headerLabel = "default Sequence header", std::string nodeLabel = "Sequence");

	virtual void drawDataFull(util::NodeBuilder& builder);
	virtual void drawDataSetValues(util::NodeBuilder& builder);

	virtual int maxLenghtOfData();

};

