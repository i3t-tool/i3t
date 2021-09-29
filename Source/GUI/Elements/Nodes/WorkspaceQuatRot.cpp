//
// Created by Sofie on 05.05.2021.
//

#include "WorkspaceQuatRot.h"

WorkspaceQuatRot::WorkspaceQuatRot()
    :   WorkspaceQuat(Core::Builder::createTransform<Core::QuatRot>())
{}


bool WorkspaceQuatRot::drawDataFull(DIWNE::Diwne &diwne, int index){
  const glm::quat& coreData = m_nodebase->as<Core::QuatRot>()->getQuat();

	// \todo JH wrong datamap
  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();

  bool inner_interaction_happen = false;
  bool valueChanged = false, actualValueChanged = false;;
	glm::quat localData;

  ImGui::PushItemWidth(getDataItemsWidth(diwne));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

  for (int columns = 0; columns < 4; columns++)
  {
    localData[columns] = coreData[columns];
    inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, actualValueChanged, localData[columns], 1, fmt::format("##{}:{}", getId(), columns)); /* todo use datamap */
    if (actualValueChanged) valueChanged = true;
    if (columns < 3) ImGui::SameLine();

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

  ImGui::PushItemWidth(getDataItemsWidth(diwne));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

  const glm::quat& coreDataNorm = m_nodebase->as<Core::QuatRot>()->getNormalized();

  for (int columns = 0; columns < 4; columns++)
  {
    localData[columns] = coreDataNorm[columns];
    /* \todo JH realy not movable normalized */
    drawDragFloatWithMap_Inline(diwne, actualValueChanged, localData[columns], 0, fmt::format("##{}:{}", getId(), columns));
    if(columns < 3) ImGui::SameLine();
  }

  ImGui::PopStyleVar();
  ImGui::PopStyleVar();
  ImGui::PopItemWidth();

  ImGui::Spring(0);
  return inner_interaction_happen;
}
