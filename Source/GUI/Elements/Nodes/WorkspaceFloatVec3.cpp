//
// Created by Sofie on 06.05.2021.
//

#include "WorkspaceFloatVec3.h"

WorkspaceFloatVec3::WorkspaceFloatVec3(ImTextureID headerBackground, WorkspaceFloatVec3Args const& args)
    : WorkspaceNodeWithCoreData(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, /*.nodebase = args.nodebase*/ })
{
  fw.showMyPopup = false;
  fw.id = "";
  fw.value = NAN;
  fw.name = "floatVec3";
  fw.rows = 0;
  fw.columns = 0;
  setDataItemsWidth();
}

WorkspaceFloatVec3::WorkspaceFloatVec3(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel)
    : WorkspaceNodeWithCoreData(headerBackground, nodebase, headerLabel, nodeLabel)
{
  fw.showMyPopup = false;
  fw.id = "";
  fw.value = NAN;
  fw.name = "floatVec3";
  fw.rows = 0;
  fw.columns = 0;
  setDataItemsWidth();
}


void WorkspaceFloatVec3::drawDataFull(util::NodeBuilder& builder, int index)
{
	if(index == 0){ // -> float

    fw.name = "float";
    fw.columns = 0;

    //assert if operator its imposible to value change (except free operators)
    const float coreData = m_nodebase->getData(index).getFloat();
    int const idOfNode = this->m_id.Get();
    int const map = m_nodebase->getDataMapRef()[0];

    bool valueChanged = false;
    float localData;

    ImGui::PushItemWidth(m_dataItemsWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {I3T::getSize(ESize::Nodes_floatPaddingX), I3T::getSize(ESize::Nodes_floatPaddingY)});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { I3T::getSize(ESize::Nodes_ItemsSpacingX), I3T::getSize(ESize::Nodes_ItemsSpacingY) });

    localData = coreData;
    valueChanged |= drawDragFloatWithMap_Inline(&localData, map, fmt::format("##{}:{}", idOfNode, index)); /* datamap value 1 is changeable */

    if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
    {
      fw.showMyPopup = true;
      fw.id = fmt::format("##{}", idOfNode);
      fw.value = localData;
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

    if (valueChanged)
    {
      setDataItemsWidth();
    }
    ImGui::Spring(0);

	}else if(index == 1){  // -> vector3

    fw.name = "vector3";

    const glm::vec3& coreData = m_nodebase->getData(index).getVec3();
    int const coreMap[3] = {1,2,3}; /* \todo JH will be map for vector? */
    int const idOfNode = this->m_id.Get();

    bool valueChanged = false;
    glm::vec3 localData;

    ImGui::PushItemWidth(m_dataItemsWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {I3T::getSize(ESize::Nodes_floatPaddingX), I3T::getSize(ESize::Nodes_floatPaddingY)});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {I3T::getSize(ESize::Nodes_ItemsSpacingX), I3T::getSize(ESize::Nodes_ItemsSpacingY)});

    for (int columns = 0; columns < 3; columns++)
    {
      localData[columns] = coreData[columns];
      valueChanged |=
          drawDragFloatWithMap_Inline(&localData[columns], coreMap[columns], fmt::format("##{}:{}", idOfNode, columns));

      if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
      {
        fw.showMyPopup = true;
        fw.id = fmt::format("##{}:{}", idOfNode, columns);
        fw.value = localData[columns];
        fw.columns = columns;
      }
      if(columns != 2)
      {
        ImGui::SameLine();
      }
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

    if (valueChanged)
    {
      setDataItemsWidth();
    }
    ImGui::Spring(0);
	}

}

int WorkspaceFloatVec3::maxLenghtOfData()
{
	if(fw.name == "float"){
    return numberOfCharWithDecimalPoint( m_nodebase->getData(0).getFloat(), m_numberOfVisibleDecimal );
	}else if(fw.name == "vector3") {
    int act, maximal = 0;
    const glm::vec3& coreData = m_nodebase->getData(1).getVec3();

    for(int column=0; column < 3; column++)
    {
      act = numberOfCharWithDecimalPoint( coreData[column], m_numberOfVisibleDecimal );
      if(act > maximal)
      {
        maximal = act;
      }
    }
    return maximal;
	}else{
		return 0;
	}
}