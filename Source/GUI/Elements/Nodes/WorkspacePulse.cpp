//
// Created by Sofie on 15.05.2021.
//

#include "WorkspacePulse.h"

WorkspacePulse::WorkspacePulse(ImTextureID headerBackground, WorkspacePulseArgs const& args)
    : WorkspaceNodeWithCoreData(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{
  fw.showMyPopup = false;
  fw.id = "";
  fw.value = NAN;
  fw.name = "pulse";
  fw.rows = 0;
  fw.columns = 0;
  setDataItemsWidth();
}

WorkspacePulse::WorkspacePulse(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceNodeWithCoreData(headerBackground, Core::Builder::createNode<ENodeType::Pulse>(), headerLabel, nodeLabel)
{
  fw.showMyPopup = false;
  fw.id = "";
  fw.value = NAN;
  fw.name = "pulse";
  fw.rows = 0;
  fw.columns = 0;
  setDataItemsWidth();
}


void WorkspacePulse::drawDataFull(util::NodeBuilder& builder, int index)
{

  ImGui::PushItemWidth(m_dataItemsWidth);

	if (ImGui::Button("Pulse"))
	{
    m_nodebase->pulse(I3T_OUTPUT0);
	}

  ImGui::PopItemWidth();

}

void WorkspacePulse::drawDataSetValues(util::NodeBuilder& builder){
    drawDataFull(builder, 0);
}

int WorkspacePulse::maxLenghtOfData()
{
  return 1;
}