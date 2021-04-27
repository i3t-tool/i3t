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
    ImRect m_dataRect; /* is set in every frame - depend on levelOfDetail of inner Nodes */
   // std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceTransformation;
public:
    std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceTransformation;
	WorkspaceSequence(ImTextureID headerBackground, WorkspaceSequenceArgs const& args);
    WorkspaceSequence(ImTextureID headerBackground, std::string headerLabel = "default Sequence header", std::string nodeLabel = "Sequence");
    std::vector<ImRect> m_dataRects; /* rects of individual */

    bool isSequence();
    int getInnerPosition(ImVec2 point);
    ImVec2 getDataSize();

    void popNode(Ptr<WorkspaceNodeWithCoreData> node);
    void pushNode(Ptr<WorkspaceNodeWithCoreData> node, int index = 0);


    std::vector<Ptr<WorkspaceNodeWithCoreData>> const& getInnerWorkspaceNodes() const;

    void drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin=nullptr, bool withPins=true);
	void drawDataFull(util::NodeBuilder& builder);
    void drawDataSetValues(util::NodeBuilder& builder);

	int maxLenghtOfData();
};

