//
// Created by Sofie on 07.05.2021.
//

#pragma once
#include "WorkspaceElementsWithCoreData.h"
class WorkspaceCycle : public WorkspaceNodeWithCoreDataWithPins
{
public:
    //===-- Double dispatch ---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<WorkspaceCycle>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

  WorkspaceCycle(Ptr<Core::NodeBase> nodebase = Core::GraphManager::createCycle(), bool drawPins=true);
	bool isCycle();

  bool middleContent(DIWNE::Diwne& diwne);
  void drawMenuLevelOfDetail();

    int maxLenghtOfData();
};

