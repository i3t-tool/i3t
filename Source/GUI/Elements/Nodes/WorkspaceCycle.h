//
// Created by Sofie on 07.05.2021.
//

#pragma once
#include "WorkspaceElementsWithCoreData.h"
class WorkspaceCycle : public WorkspaceNodeWithCoreDataWithPins
{
public:
	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<WorkspaceCycle>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	WorkspaceCycle(DIWNE::Diwne& diwne, Ptr<Core::Node> nodebase = Core::GraphManager::createCycle(),
	               bool drawPins = true);
	bool isCycle();

	bool buttonStepNext();
	bool buttonStepBack();
	bool buttonStopAndReset();
	bool buttonPlayPause();

	bool topContent();
	bool middleContent();
	bool leftContent();
	bool rightContent();

	void drawMenuLevelOfDetail();

	int maxLengthOfData();
};
