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
protected:
    ImVec2 m_dataSize; /* is set in every frame - depend on levelOfDetail of inner Nodes */
    std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceTransformation;
public:
	WorkspaceSequence(ImTextureID headerBackground, WorkspaceSequenceArgs const& args);
    WorkspaceSequence(ImTextureID headerBackground, std::string headerLabel = "default Sequence header", std::string nodeLabel = "Sequence");

    bool isSequence();

    std::vector<Ptr<WorkspaceNodeWithCoreData>> const& getInnerWorkspaceNodes() const;

    void drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin);
	void drawDataFull(util::NodeBuilder& builder);
    void drawDataSetValues(util::NodeBuilder& builder);

	int maxLenghtOfData();
};

