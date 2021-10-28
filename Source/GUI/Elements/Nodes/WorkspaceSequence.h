#pragma once
#include "WorkspaceElementsWithCoreData.h"
#include "WorkspaceTransformation.h"

class WorkspaceSequence : public WorkspaceNodeWithCoreData
{
protected:
    int m_position_of_dummy_data = -1;
    ImVec2 m_sizeOfDummy = ImVec2(100,1); /* \todo JH width from some setting */
    std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceInnerTransformations;
public:
    WorkspaceSequence(Ptr<Core::NodeBase> nodebase = Core::Builder::createSequence());

    void setPostionOfDummyData(int positionOfDummyData = -1);

    bool isSequence();
    int getInnerPosition(ImVec2 point);
    int getInnerPosition(std::vector<ImVec2> points);

    void popNode(Ptr<WorkspaceNodeWithCoreData> node);
    void pushNode(Ptr<WorkspaceNodeWithCoreData> node, int index = 0);

    void moveNodeToSequenceWithCheck(DIWNE::Diwne &diwne, Ptr<WorkspaceNodeWithCoreData> dragedNode, int index);


    std::vector<Ptr<WorkspaceNodeWithCoreData>> const& getInnerWorkspaceNodes() const;


    bool drawDataFull(DIWNE::Diwne &diwne, int index);

    void drawMenuLevelOfDetail();

	int maxLenghtOfData();
};

