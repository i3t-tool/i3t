//
// Created by Sofie on 15.05.2021.
//

#include "WorkspacePulse.h"

WorkspacePulse::WorkspacePulse()
    : WorkspaceNodeWithCoreData(Core::Builder::createNode<ENodeType::Pulse>())
{
  setDataItemsWidth();
}

bool WorkspacePulse::drawDataFull(DIWNE::Diwne& diwne, int index)
{

  ImGui::PushItemWidth(getDataItemsWidth(diwne));

	if (ImGui::Button("Pulse"))
	{
    m_nodebase->pulse(I3T_OUTPUT0);
	}

  ImGui::PopItemWidth();
  return false;

}

int WorkspacePulse::maxLenghtOfData(int index)
{
  return 1;
}
