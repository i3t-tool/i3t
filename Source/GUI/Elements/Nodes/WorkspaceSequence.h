#pragma once
#include "WorkspaceElementsWithCoreData.h"
//#include "WorkspaceMatrixTranslation.h"

class WorkspaceSequence : public WorkspaceNodeWithCoreData
{
protected:
    int m_position_of_dummy_data = -1;
    ImVec2 m_sizeOfDummy = ImVec2(20,1); /* \todo JH width from some setting */
    std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceTransformation;
public:
    WorkspaceSequence(Ptr<Core::NodeBase> nodebase = Core::Builder::createSequence());

    void setPostionOfDummyData(int positionOfDummyData = -1);

    bool isSequence();
    int getInnerPosition(ImVec2 point);
    int getInnerPosition(std::vector<ImVec2> points);

    void popNode(Ptr<WorkspaceNodeWithCoreData> node);
    void pushNode(Ptr<WorkspaceNodeWithCoreData> node, int index = 0);


    std::vector<Ptr<WorkspaceNodeWithCoreData>> const& getInnerWorkspaceNodes() const;


bool drawDataFull(DIWNE::Diwne &diwne, int index);

	int maxLenghtOfData(int index=0);
};

