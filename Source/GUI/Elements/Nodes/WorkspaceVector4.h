#pragma once
#include "WorkspaceElementsWithCoreData.h"

class WorkspaceVector4 : public WorkspaceNodeWithCoreData
{
public:
    WorkspaceVector4(Ptr<Core::NodeBase> nodebase = Core::Builder::createNode<ENodeType::Vector4ToVector4>());

    virtual bool drawDataFull(DIWNE::Diwne& diwne, int index);

	int maxLenghtOfData(int index=0);
};
