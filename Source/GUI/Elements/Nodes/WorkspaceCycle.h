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

    bool buttonStepNext(ImVec2 const &  button_sz);
    bool buttonStepBack(ImVec2 const &  button_sz);
    bool buttonStopAndReset(ImVec2 const &  button_sz);
    bool buttonPlayPause(ImVec2 const &  button_sz);

  bool middleContent(DIWNE::Diwne& diwne);
  bool leftContent(DIWNE::Diwne& diwne);
  bool rightContent(DIWNE::Diwne& diwne);

  void drawMenuLevelOfDetail();

    int maxLenghtOfData();
};

