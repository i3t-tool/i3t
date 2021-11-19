#pragma once
#include "WorkspaceElementsWithCoreData.h"
#include "WorkspaceTransformation.h"

class WorkspaceSequence : public WorkspaceNodeWithCoreDataWithPins
{
protected:
    int m_position_of_dummy_data = -1;
    ImVec2 m_sizeOfDummy = ImVec2(100,1); /* \todo JH width from some setting */

    bool m_drawPins;
    std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceInnerTransformations;
public:
    WorkspaceSequence(Ptr<Core::NodeBase> nodebase = Core::Builder::createSequence(), bool drawPins=true);

    void setPostionOfDummyData(int positionOfDummyData = -1);

    bool isSequence();
    int getInnerPosition(ImVec2 point);
    int getInnerPosition(std::vector<ImVec2> points);

    void popNode(Ptr<WorkspaceNodeWithCoreData> node);
    void pushNode(Ptr<WorkspaceNodeWithCoreData> node, int index = 0);

    void moveNodeToSequence(DIWNE::Diwne &diwne, Ptr<WorkspaceNodeWithCoreData> dragedNode, int index=0);
    void moveNodeToWorkspaceWindow(DIWNE::Diwne &diwne, Ptr<WorkspaceNodeWithCoreData> dragedNode);


    std::vector<Ptr<WorkspaceNodeWithCoreData>> const& getInnerWorkspaceNodes() const;

    bool topContent(DIWNE::Diwne &diwne);
    bool middleContent(DIWNE::Diwne &diwne);

    void drawMenuLevelOfDetail();

	int maxLenghtOfData();
};

