#pragma once
#include "WorkspaceElementsWithCoreData.h"
#include "WorkspaceSequence.h"

#include "Core/Nodes/GraphManager.h"
class WorkspaceCamera : public WorkspaceNodeWithCoreData
{
protected:
    Ptr<WorkspaceSequence> m_projection = nullptr;
    Ptr<WorkspaceSequence> m_view = nullptr;
public:
    WorkspaceCamera();

    bool isCamera();

    Ptr<WorkspaceSequence> const& getProjection() const {return m_projection;};
    Ptr<WorkspaceSequence> const& getView() const {return m_view;};

    bool drawDataFull(DIWNE::Diwne &diwne, int index=0){return false;}; /* camera has no data */

    bool middleContent(DIWNE::Diwne &diwne);

	int maxLenghtOfData(int index=0);
};


