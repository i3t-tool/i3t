#pragma once

#include "WorkspaceElementsWithCoreData.h"
#include "WorkspaceSequence.h"

#include "Core/Nodes/GraphManager.h"

class WorkspaceCamera : public WorkspaceNodeWithCoreDataWithPins
{
protected:
    Ptr<WorkspaceSequence> m_projection = nullptr;
    Ptr<WorkspaceSequence> m_view = nullptr;
public:
	WorkspaceCamera(DIWNE::Diwne& diwne);

	//===-- Double dispatch ---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<WorkspaceSequence>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

    bool isCamera();

    Ptr<WorkspaceSequence> const& getProjection() const {return m_projection;};
    Ptr<WorkspaceSequence> const& getView() const {return m_view;};

    //bool drawDataFull(DIWNE::Diwne &diwne){return false;}; /* camera has no data */

    bool middleContent();

    void drawMenuLevelOfDetail();

	int maxLenghtOfData();

//	bool leftContent(DIWNE::Diwne &diwne);
//	bool rightContent(DIWNE::Diwne &diwne);
};


