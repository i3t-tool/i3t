//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatRot.h"

WorkspaceQuatRot::WorkspaceQuatRot(ImTextureID headerBackground, WorkspaceQuatRotArgs const& args)
    : WorkspaceQuat(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createTransform<Core::Translation>() })
{}

WorkspaceQuatRot::WorkspaceQuatRot(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceQuat(headerBackground, Core::Builder::createTransform<Core::QuatRot>(), headerLabel, nodeLabel)
{}

void WorkspaceQuatRot::drawDataSetValues(util::NodeBuilder& builder)
{
	drawDataFull(builder, 0);
}

void WorkspaceQuatRot::drawDataFull(util::NodeBuilder& builder, int index){
  const glm::quat& coreData = m_nodebase->getData(index).getQuat();

	//wrong datamap
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
  int const idOfNode = this->m_id.Get();

  bool valueChanged = false;
	glm::quat localData;

  ImGui::PushItemWidth(m_dataItemsWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

  for (int columns = 0; columns < 4; columns++)
  {
    localData[columns] = coreData[columns];
    valueChanged |=
        drawDragFloatWithMap_Inline(&localData[columns], 1, fmt::format("##{}:{}", idOfNode, columns));

    if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
    {
      fw.showMyPopup = true;
      fw.id = fmt::format("##{}:{}", idOfNode, columns);
      fw.value = localData[columns];
      fw.columns = columns;
    }
    if(columns != 3){
      ImGui::SameLine();
    }
  }
  ImGui::PopStyleVar();
  ImGui::PopStyleVar();
  ImGui::PopItemWidth();

  if (valueChanged)
  {
    m_nodebase->setValue(localData);
    setDataItemsWidth();
  }

	ImGui::Text("Normalized:");

  ImGui::PushItemWidth(m_dataItemsWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

  const glm::quat& coreDataNorm = m_nodebase->as<Core::QuatRot>()->getNormalized();

  for (int columns = 0; columns < 4; columns++)
  {
    localData[columns] = coreDataNorm[columns];
    valueChanged |=
        drawDragFloatWithMap_Inline(&localData[columns], coreMap[columns], fmt::format("##{}:{}", idOfNode, columns));

    if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
    {
      fw.showMyPopup = true;
      fw.id = fmt::format("##{}:{}", idOfNode, columns);
      fw.value = localData[columns];
      fw.columns = columns;
    }
    if(columns != 3){
      ImGui::SameLine();
    }
  }

  ImGui::PopStyleVar();
  ImGui::PopStyleVar();
  ImGui::PopItemWidth();

  ImGui::Spring(0);
}