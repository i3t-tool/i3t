#pragma once
#include "WorkspaceElementsWithCoreData.h"

/*! \class WorkspaceMatrix4x4 virtual class for all Nodes with Matrix4x4 Core data*/
class WorkspaceMatrix4x4 : public WorkspaceNodeWithCoreData
{
public:
    WorkspaceMatrix4x4(Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Matrix>());

    virtual bool drawDataFull(DIWNE::Diwne &diwne, int index=0);
	int maxLenghtOfData(int index=0);
};

extern int maxLenghtOfData4x4(const glm::mat4& data, int numberOfVisibleDecimal);
