#pragma once
#include "WorkspaceElementsWithCoreData.h"
#include "WorkspaceTransformation.h"

class WorkspaceSequence : public WorkspaceNodeWithCoreData
{
protected:
    int m_position_of_dummy_data = -1;
    ImVec2 m_sizeOfDummy = ImVec2(100,1); /* \todo JH width from some setting */
    std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceInnerTransformations;

    std::vector<Ptr<WorkspaceCorePin>>    m_workspaceInputs;
	std::vector<Ptr<WorkspaceCorePin>>    m_workspaceOutputs;
public:
    WorkspaceSequence(Ptr<Core::NodeBase> nodebase = Core::Builder::createSequence());


    std::vector<Ptr<WorkspaceCorePin>> const& getInputs() const {return m_workspaceInputs;};
	std::vector<Ptr<WorkspaceCorePin>> const& getOutputs() const {return m_workspaceOutputs;};

    void setPostionOfDummyData(int positionOfDummyData = -1);

    bool isSequence();
    int getInnerPosition(ImVec2 point);
    int getInnerPosition(std::vector<ImVec2> points);

    void popNode(Ptr<WorkspaceNodeWithCoreData> node);
    void pushNode(Ptr<WorkspaceNodeWithCoreData> node, int index = 0);

    void moveNodeToSequence(DIWNE::Diwne &diwne, Ptr<WorkspaceNodeWithCoreData> dragedNode, int index=0);
    void moveNodeToWorkspaceWindow(DIWNE::Diwne &diwne, Ptr<WorkspaceNodeWithCoreData> dragedNode);


    std::vector<Ptr<WorkspaceNodeWithCoreData>> const& getInnerWorkspaceNodes() const;

    bool middleContent(DIWNE::Diwne &diwne);
    bool leftContent(DIWNE::Diwne &diwne);
    bool rightContent(DIWNE::Diwne &diwne);

    void drawMenuLevelOfDetail();

	int maxLenghtOfData();
};

