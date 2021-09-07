#pragma once
#include "WorkspaceElementsWithCoreData.h"

class WorkspaceVector3 : public WorkspaceNodeWithCoreData
{
public:
    WorkspaceVector3(Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector3>());

	virtual bool drawDataFull(DIWNE::Diwne& diwne, int index);
	int maxLenghtOfData(int index=0);
};

