#pragma once
#include "WorkspaceElementsWithCoreData.h"

/*! \class WorkspaceFloat virtual class for all Nodes with Float Core data*/
class WorkspaceFloat : public WorkspaceNodeWithCoreData
{
public:
    WorkspaceFloat(Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Float>());

    virtual bool drawDataFull(DIWNE::Diwne &diwne, int index=0);

    int maxLenghtOfData(int index=0);
};
